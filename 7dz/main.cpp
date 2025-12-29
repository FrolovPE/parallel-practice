#include <iostream>
#include <pthread.h>
#include "pll.h"
#include "args.h"
#include "mytime.h"


int main(int argc, char* argv[])
{
    int p{};
    int k = 0;
    int status;
    // int errsum = 0;
    // int totalres = 0;
    long long n{};
    // double curr{};
    double totalelapsed = 0;

    if(!(argc == 3 && sscanf(argv[1],"%d",&p) == 1 && sscanf(argv[2],"%lld",&n) == 1 ))
    {
        printf("Usage: %s <p> <n>  \n",argv[0]);
        return 0;
    }

    if(p <= 0 || n <=0)
    {
        printf("Error: p or n <=0\n");
        return -3;
    }

    // printf("size of ull %ld and size of ulli %ld",sizeof(unsigned long long),sizeof(unsigned long long int));
    
    
    

    // printf("p = %d n1 = %d n2 = %d name = %s\n",p,n1,n2,name);
     
    pthread_t *tid = new pthread_t[p];

    // int *err = new int[p];

    

    // for(k = 0; k < p ; k++) 
    // {
    //     err[k]=0;
    // }

    

    unsigned long long int N = 10000;

    args *a = new args[p];


    pthread_barrier_t barrier;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

    pthread_barrier_init(&barrier,0,p);
    unsigned long long int global_sum{};
    unsigned long long found{};
    bool find = false;
    unsigned long long start{},end{};
    

    for(k = 0 ; k < p; k++)
    {
        a[k].n = n;
        a[k].N = N;
        a[k].p = p;
        a[k].start = &start;
        a[k].end = &end;
        a[k].k = k;
        a[k].find = &find;
        a[k].found = &found;
        a[k].all = a;
        a[k].global_sum = &global_sum;
        a[k].barrier = &barrier;
        a[k].mutex = &mutex;
        a[k].mutex1 = &mutex1;
        a[k].totalelapsed = &totalelapsed;
        

    }


    

    for (k = 1; k < p; k++) 
    {
        status = pthread_create(&tid[k], 0, thread_func, (void*) &a[k]);

        if (status != 0)
        {
                printf("main error: can't create thread, status = %d\n", status);
                delete []tid;
                delete []a;
                pthread_barrier_destroy(&barrier);
                pthread_mutex_destroy(&mutex);
                pthread_mutex_destroy(&mutex1);
                return -11;

        }
    }

    thread_func(a+0);


    for (k = 1; k < p; k++) 
    {
        status = pthread_join(tid[k],0);
        if (status != 0)
        {
                printf("error in pthread_join, status = %d\n", status);
                delete []tid;
                delete []a;
                pthread_barrier_destroy(&barrier);
                pthread_mutex_destroy(&mutex);
                pthread_mutex_destroy(&mutex1);
                return -12;
        }


        // elapsed = get_full_time() - elapsed;
        // printf("CPU Time thread %d = %.2lf\n",k,elapsed);
    }

    printf("Total Time = %.2lf\n",totalelapsed);

    // printf("s = %d\n",errsum);
    // if(errsum < 0) 
    // {
    //     delete []arr;
    //     delete []tid;
    //     delete []err;
    //     delete []a;
    //     delete []carr;
    //     pthread_barrier_destroy(&barrier);
    //     return -1;
    // }

    

    

    

    // for (k = 0; k < p; k++) 
    // {
    //     totalres += a[k].res;
    //     // printf("thread %d a[%d].res = %d\n", k,k, a[k].res);

    // }

    printf("RESULT %d: %lld\n ",p,global_sum);

    // printf("%d\n",31546220-31498168);

   
            
    
    delete []tid;
    delete []a;
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex1);
    return 0;
}