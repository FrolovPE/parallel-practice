#include <iostream>
#include <pthread.h>
#include "pll.h"
#include "args.h"


int main(int argc, char* argv[])
{
    int p = argc-1;
    int k{};
    int status{};
    int errsum = 0;
    int totalres = 0;

    if(argc == 1)
    {
        printf("Usage: %s <files>\n",argv[0]);
        return 0;
    }
     
    pthread_t *tid = new pthread_t[p];

    int *err = new int[p];
    double *locMin = new double[p];

    for(k = 0; k < p ; k++) 
    {
        locMin[k] = 0;
        err[k]=0;
    }
    args *a = new args[p];

    pthread_barrier_t barrier;

    pthread_barrier_init(&barrier,0,p);

    double globalMin{};

    for(k = 0 ; k < p; k++)
    {
        a[k].name = argv[k+1];
        a[k].k = k;
        a[k].p = p;
        a[k].err = err;
        a[k].locMin = locMin;
        a[k].globalMin = &globalMin;
        a[k].errsum = &errsum;
        a[k].barrier = &barrier;

    }



    for (k = 1; k < p; k++) 
    {
        status = pthread_create(&tid[k], 0, thread_func, (void*) &a[k]);

        if (status != 0)
        {
                printf("main error: can't create thread, status = %d\n", status);
                delete []locMin;
                delete []tid;
                delete []err;
                delete []a;
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
                delete []locMin;
                delete []tid;
                delete []err;
                delete []a;
                return -12;
        }


    }
    // printf("s = %d\n",errsum);
    if(errsum < 0) 
    {
        delete []locMin;
        delete []tid;
        delete []err;
        delete []a;
        pthread_barrier_destroy(&barrier);
        return -1;
    }

    

    

    

    for (k = 0; k < p; k++) 
    {
        totalres += a[k].res;
        // printf("thread %d a[%d].res = %d\n", k,k, a[k].res);

    }

    printf("Total result : %d\n",totalres);
    delete []locMin;
    delete []tid;
    delete []err;
    delete []a;
    pthread_barrier_destroy(&barrier);
    return 0;
}