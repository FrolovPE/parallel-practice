#include "pll.h"
#include "args.h"
#include "mytime.h"
#include <cmath>
#define EPS 1e-15

bool prime(unsigned long long x)
{
    if(x<2) return false;
    if(x % 2 == 0) return x==2;
    

    for(int i = 3; i*i <= x; i+=2)
    {
        if(x%i == 0) return false;
    }
    return true;
}

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;

    int n = a->n;
    int k = a->k;
    int p = a->p;
    int N = a->N;
    args *all = a->all;
    pthread_barrier_t *barrier = a->barrier;
    pthread_mutex_t *mutex = a->mutex;

    double elapsed = get_full_time();

    (void)barrier;
    (void)p;
    (void)k;
    (void)N;

    unsigned long long local_sum{};
    long long proc{};
    int start = -1e64;

    (void)n;
    (void)local_sum;
    (void)proc;

    pthread_mutex_lock(mutex);
    for(int i = 0; i < p;i++)
    {
        if(start < all[i].end)
        {
            start = all[i].end;
        }
    }
    a->end = start + N;
    pthread_mutex_unlock(mutex);

    start = (start % 2 == 0) ? start+1:start;

    printf("thread %d start = %d end = %d\n",k,start,a->end);
    // unsigned long long first{};
    // unsigned long long sec{};

    for(int i = start; i < a->end; i+=2)
    {
        if(prime(i) && prime(i+6))
        {
            printf("thread %d first = %d sec = %d\n",k,i,i+6);
            local_sum += 2*i+6;
            proc++;
            a->lastprime = i + 6;
        }

        if(proc >= n)
        {
            pthread_mutex_lock(mutex);
            *a->global_sum += local_sum;
            *a->found += proc;
            *a->find = true;
            pthread_mutex_unlock(mutex);
            break;
        }
    }

    


    pthread_mutex_lock(mutex);

    
    printf("thread %d here\n",k);

    *a->global_sum += local_sum;
    *a->found += proc;
    if(*a->found >= n)
    {
        for(int i = a->lastprime; *a->found > n; i-=2)
        {
            if(prime(i) && prime(i-6))
            {
                *a->global_sum -= 2*i-6;
                *a->found-=1;
            }
            
        }
    }
    *a->find = true;
    pthread_mutex_unlock(mutex);

    if(*a->find)
    {
        return (void*)(SUCCESS);
    }

    

   


    elapsed = get_full_time() - elapsed;
    pthread_mutex_lock(mutex);
    *a->totalelapsed += elapsed;
    pthread_mutex_unlock(mutex);
    printf("CPU Time thread %d = %.2lf\n",k,elapsed);

    
    return (void*)SUCCESS;
    
}