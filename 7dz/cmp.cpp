// task7_5.cpp
// g++ -O2 -pthread -std=c++17 task7_5.cpp -o a.out
// ./a.out p n
//
// Считает сумму первых n "sexy prime pairs" (p, p+6), где оба простые.
// Пара считается как (p, p+6), в сумму добавляется p + (p+6).
//
// Требование "зоны длины N": здесь зона = BLOCK чисел (фиксированная длина).
// Поток берет себе зону, отрабатывает, затем ищет ближайшую свободную зону (вперед)
// и берет её. Балансировка нагрузки — динамическая.

#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <map>
#include <atomic>
#include <ctime>
#include <algorithm>

// -------------------- настройки --------------------
static constexpr uint64_t BLOCK = 200000; // длина зоны (фиксированная). Можно менять.

// -------------------- время --------------------
static inline timespec ts_now(clockid_t clk) {
    timespec t{};
    clock_gettime(clk, &t);
    return t;
}
static inline double ts_sec(const timespec& a, const timespec& b) {
    // b - a
    long long sec = (long long)b.tv_sec - (long long)a.tv_sec;
    long long nsec = (long long)b.tv_nsec - (long long)a.tv_nsec;
    return (double)sec + (double)nsec * 1e-9;
}

// -------------------- Miller-Rabin для uint64 --------------------
static inline uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod) {
    __uint128_t x = ( __uint128_t )a * ( __uint128_t )b;
    return (uint64_t)(x % mod);
}
static inline uint64_t mod_pow_u64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mod_mul_u64(r, a, mod);
        a = mod_mul_u64(a, a, mod);
        d >>= 1;
    }
    return r;
}
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (uint64_t p : small_primes) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    // n-1 = d * 2^s
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    auto witness = [&](uint64_t a)->bool{
        if (a % n == 0) return false;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1) return false;
        for (uint64_t i = 1; i < s; ++i) {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) return false;
        }
        return true; // составное
    };

    // Набор баз, широко используемый на практике для 64-бит.
    // Для учебной задачи обычно более чем достаточно.
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (uint64_t a : bases) {
        if (witness(a)) return false;
    }
    return true;
}

// -------------------- планировщик зон --------------------
struct Scheduler {
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    std::vector<unsigned char> state; // 0 = свободна, 1 = взята
    // Берём "ближайшую свободную" начиная с (last_chunk + 1) вперёд.
    int acquire_next(int last_chunk, bool stop) {
        if (stop) return -1;
        pthread_mutex_lock(&mtx);

        // Ищем ближайшую свободную после last_chunk
        int start = last_chunk + 1;
        for (int i = start; i < (int)state.size(); ++i) {
            if (state[i] == 0) {
                state[i] = 1;
                pthread_mutex_unlock(&mtx);
                return i;
            }
        }
        // Не нашли — создаём новую зону в конце и сразу берём её
        state.push_back(1);
        int id = (int)state.size() - 1;
        pthread_mutex_unlock(&mtx);
        return id;
    }
};

// -------------------- общие данные --------------------
struct Shared {
    uint64_t n_target = 0;
    std::atomic<uint64_t> total_pairs_found{0};
    std::atomic<bool> stop{false};

    Scheduler sched;

    pthread_mutex_t results_mtx = PTHREAD_MUTEX_INITIALIZER;
    std::map<int, std::vector<unsigned long long>> segment_pairs; // seg_id -> list of (p + p+6) in seg, по возрастанию p
};

struct ThreadArg {
    int tid = 0;
    Shared* sh = nullptr;
    double cpu_time_sec = 0.0;
};

static void process_segment(int seg_id, Shared* sh, std::vector<unsigned long long>& out) {
    uint64_t L = 2ULL + (uint64_t)seg_id * BLOCK;
    uint64_t R = L + BLOCK - 1;

    // Чтобы не считать чётные (кроме 2)
    uint64_t x = (L <= 2 ? 2 : L);
    if (x > 2 && (x % 2 == 0)) x++;

    for (; x <= R; x += (x == 2 ? 1 : 2)) {
        // x и x+6 оба простые
        if (is_prime_u64(x) && is_prime_u64(x + 6)) {
            unsigned long long s = (unsigned long long)x + (unsigned long long)(x + 6);
            out.push_back(s);
        }
        if (sh->stop.load(std::memory_order_relaxed)) {
            // уже хватает пар, можно не упираться (но сегмент доделывать не обязаны)
            // оставим быстрый выход.
            // Важно: сегменты идут по возрастанию, но мы всё равно собираем "с запасом".
            break;
        }
    }
}

static void* thread_func(void* ptr) {
    ThreadArg* a = (ThreadArg*)ptr;
    Shared* sh = a->sh;

    timespec t0 = ts_now(CLOCK_THREAD_CPUTIME_ID);

    int last = -1;
    while (true) {
        int seg = sh->sched.acquire_next(last, sh->stop.load(std::memory_order_relaxed));
        if (seg < 0) break;
        last = seg;

        std::vector<unsigned long long> local;
        local.reserve(256);
        process_segment(seg, sh, local);

        // Публикуем результат сегмента
        pthread_mutex_lock(&sh->results_mtx);
        sh->segment_pairs.emplace(seg, std::move(local));
        pthread_mutex_unlock(&sh->results_mtx);

        // Обновим общий счётчик пар (сколько нашли в этом сегменте)
        // (Берём count уже из map нельзя — local moved. Поэтому считаем до move.)
        // Решение: посчитать заранее.
        // Но local уже перемещён — значит посчитаем иначе: быстро возьмём размер из map под тем же локом.
        uint64_t add = 0;
        pthread_mutex_lock(&sh->results_mtx);
        auto it = sh->segment_pairs.find(seg);
        if (it != sh->segment_pairs.end()) add = (uint64_t)it->second.size();
        pthread_mutex_unlock(&sh->results_mtx);

        uint64_t total = sh->total_pairs_found.fetch_add(add) + add;
        if (total >= sh->n_target) {
            sh->stop.store(true);
            // не выходим сразу — цикл всё равно не возьмёт новые сегменты из-за stop
        }
    }

    timespec t1 = ts_now(CLOCK_THREAD_CPUTIME_ID);
    a->cpu_time_sec = ts_sec(t0, t1);
    return nullptr;
}

// -------------------- main --------------------
int main(int argc, char** argv) {
    int p = 0;
    unsigned long long n = 0;

    if (!(argc == 3 && std::sscanf(argv[1], "%d", &p) == 1 && std::sscanf(argv[2], "%llu", &n) == 1)) {
        std::printf("Usage: %s <p> <n>\n", argv[0]);
        return 0;
    }
    if (p <= 0 || n == 0) {
        std::printf("Error: p must be > 0 and n must be > 0\n");
        return 0;
    }

    Shared sh;
    sh.n_target = (uint64_t)n;

    std::vector<pthread_t> th(p);
    std::vector<ThreadArg> args(p);

    // Процессорное время всего процесса (суммарно по потокам) — через CLOCK_PROCESS_CPUTIME_ID
    timespec proc0 = ts_now(CLOCK_PROCESS_CPUTIME_ID);

    // Запускаем p-1 поток, а "0-й" выполняем в main (как в условии)
    for (int i = 0; i < p; ++i) {
        args[i].tid = i;
        args[i].sh = &sh;
    }

    for (int i = 1; i < p; ++i) {
        if (pthread_create(&th[i], nullptr, thread_func, &args[i]) != 0) {
            std::printf("pthread_create failed for thread %d\n", i);
            return 0;
        }
    }

    // main как поток 0
    thread_func(&args[0]);

    for (int i = 1; i < p; ++i) {
        pthread_join(th[i], nullptr);
    }

    timespec proc1 = ts_now(CLOCK_PROCESS_CPUTIME_ID);

    // Теперь гарантированно посчитаем сумму первых n пар по порядку сегментов
    unsigned long long res = 0;
    uint64_t need = (uint64_t)n;
    uint64_t got = 0;

    // segment_pairs — map, уже отсортирован по seg_id
    for (const auto& kv : sh.segment_pairs) {
        const auto& vec = kv.second;
        for (unsigned long long s : vec) {
            res += s;
            if (++got == need) break;
        }
        if (got == need) break;
    }

    // Вывод результата — строго как в задании
    std::printf("Result = %llu\n", res);

    // Время по потокам и суммарно
    double sum_threads = 0.0;
    for (int i = 0; i < p; ++i) sum_threads += args[i].cpu_time_sec;

    for (int i = 0; i < p; ++i) {
        std::printf("Thread %d CPU time = %.6f sec\n", i, args[i].cpu_time_sec);
    }
    std::printf("Sum of threads CPU time = %.6f sec\n", sum_threads);
    std::printf("Process CPU time = %.6f sec\n", ts_sec(proc0, proc1));

    return 0;
}
