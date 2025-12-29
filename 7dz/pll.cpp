#include "pll.h"
#include "args.h"
#include "mytime.h"
#include <cmath>
#define EPS 1e-15

bool prime(unsigned long long x)
{
    if(x<2) return false;
    if(x % 2 == 0) return x==2;
    

    for(unsigned long long  i = 3; i*i <= x; i+=2)
    {
        if(x%i == 0) return false;
    }
    return true;
}

void process(unsigned long long start,unsigned long long end,unsigned long long &local_sum,unsigned long long &local_found)
{
    local_sum = 0;
    local_found = 0;
    

    for(unsigned long long k = start; k < end; k+=2)
    {
        if(prime(k) && prime(k+6))
        {
            local_found++;
            local_sum += 2*k + 6;
        }
    }
}

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;

    int n = a->n;
    int k = a->k;
    int p = a->p;
    const unsigned long long int N = a->N;
    args *all = a->all;
    pthread_barrier_t *barrier = a->barrier;
    pthread_mutex_t *mutex = a->mutex;
    pthread_mutex_t *mutex1 = a->mutex1;

    double elapsed = get_full_time();

    (void)barrier;
    (void)p;
    (void)k;
    (void)N;

    unsigned long long local_sum{};
    unsigned long long local_found{};
    unsigned long long start{},end{};

    (void)n;
    (void)local_sum;

    

    

    // unsigned long long first{};
    // unsigned long long sec{};
    (void)all;
    unsigned long long ffound{};

    while(ffound <= a->n)
    {
        pthread_mutex_lock(mutex);
        start = *a->end;
        start = (start % 2 == 0) ? start+1:start;
        *a->start = start;
        *a->end = start + N;
        end = *a->end;
        // printf("thread %d start = %lld end = %lld a->start = %lld a->end = %lld \n",k,start,end,*a->start,*a->end);
        pthread_mutex_unlock(mutex);
        process(start,end,local_sum,local_found);
        // printf("local_sum %lld local_found %lld\n",local_sum,local_found);
        a->sum += local_sum;
        pthread_mutex_lock(mutex1);
        // *a->global_sum += local_sum;
        *a->found += local_found;
        a->local_f +=local_found;
        ffound = *a->found;
        pthread_mutex_unlock(mutex1);

    }

    pthread_barrier_wait(barrier);

    if(k == 0)
    {
        // printf("a->found = %lld\n",*a->found);
        unsigned long long fin_sum{};
        unsigned long long fin_num{};

         for (int i = 0; i < a->p; i++)
        {
          if(fin_num <= a->n)
           { 
            fin_sum += all[i].sum;
            fin_num += all[i].local_f;
           }

        }
        ffound = fin_num;
        if(*a->end % 2 == 1) *a->end+=1;
        unsigned long long i{};

        // printf("ffound last %lld ffound %lld fin_sum %lld\n",*a->end,ffound,fin_sum);

        int c = 0;


        for(i = *a->end-1; ffound > a->n; i-=2)
        {
            if(prime(i) && prime(i+6))
            {
                c++;
                // printf ("deleted (%llu, %llu) c = %d\n", i, i + 6,c);
                --ffound;
                fin_sum -= 2*i+6;
                
            }
        }
        *a->global_sum = fin_sum;
    }

    

    // for(unsigned long long int i = start; i < end; i+=2)
    // {
    //     if(prime(i) && prime(i+6))
    //     {
    //         printf("thread %d first = %lld sec = %lld\n",k,i,i+6);
    //         local_sum += 2*i+6;
    //         proc++;
    //         a->lastprime = i + 6;
    //     }

    //     if(proc >= n)
    //     {
    //         pthread_mutex_lock(mutex);
    //         *a->global_sum += local_sum;
    //         *a->found += proc;
    //         *a->find = true;
    //         pthread_mutex_unlock(mutex);
    //         break;
    //     }
    // }

    


    // pthread_mutex_lock(mutex);

    
    // printf("thread %d here\n",k);

    // *a->global_sum += local_sum;
    // *a->found += proc;
    // if(*a->found >= n)
    // {
    //     for(int i = a->lastprime; *a->found > n; i-=2)
    //     {
    //         if(prime(i) && prime(i-6))
    //         {
    //             *a->global_sum -= 2*i-6;
    //             *a->found-=1;
    //         }
            
    //     }
    //     *a->find = true;
        
    // }
    
    // pthread_mutex_unlock(mutex);

    

    

   


    elapsed = get_full_time() - elapsed;
    pthread_mutex_lock(mutex);
    *a->totalelapsed += elapsed;
    pthread_mutex_unlock(mutex);
    printf("CPU Time thread %d = %.2lf\n",k,elapsed);

    
    return (void*)SUCCESS;
    
}