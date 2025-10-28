#include <iostream>
#include <pthread.h>
#include "pll.h"
#include "args.cpp"


int main(int argc, char* argv[])
{
    int p = argc-1;
    int k;
    int status;

    if(argc == 1)
    {
        printf("Usage: %s <files>",argv[0]);
        return 0;
    }
     
    pthread_t *tid = new pthread_t[p];

    int *err = new int[p];

    args *a = new args[p];

    for(k = 0 ; k < p; k++)
    {
        a[k].name = argv[k+1];
        a[k].k = k;
        a[k].p = p;
        a[k].err = err;
        err[k] = 0;

    }

    // for(k = 0; k < p ; k++)
    // {
    //     printf("name a[%d] = %s\n",k,a[k].name);
    // }

    for (k = 0; k < p; k++) 
    {
        status = pthread_create(&tid[k], 0, thread_func, (void*) &a[k]);

        if (status != 0)
        {
                printf("main error: can't create thread, status = %d\n", status);
                exit(-11);

        }
    }

    for (k = 0; k < p; k++) 
    {
        status = pthread_join(tid[k],0);
        if (status != 0)
        {
                printf("error in pthread_join, status = %d\n", status);
                exit(-12);
        }

    }

    for (k = 0; k < p; k++) 
    {
        printf("thread %d a[%d].res = %d\n", k,k, a[k].res);

    }
          

    return 0;
}