#include <iostream>
#include <pthread.h>
#include "pll.h"
#include "args.h"


static pthread_barrier_t barrier;

int main(int argc, char* argv[])
{
    int p = argc-1;
    int k;
    int status;
    int s = 0;
    int totalres = 0;
    double globalVal = 0;

    if(argc == 1)
    {
        printf("Usage: %s <files>\n",argv[0]);
        return 0;
    }
     
    pthread_t *tid = new pthread_t[p];

    int *err = new int[p];

    for(k = 0; k < p ; k++) err[k]=-17;

    args *a = new args[p];

    double *locMin = new double[p];

    for(k = 0; k < p ; k++) locMin[k]=0;

    if(pthread_barrier_init(&barrier,0,p)!=0)
    {
        printf("Cant init barrier\n");
        delete []err;
        delete []a;
        delete []tid;
        delete []locMin;
        return -5;
    }

    pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

    for(k = 0 ; k < p; k++)
    {
        a[k].name = argv[k+1];
        a[k].k = k;
        a[k].p = p;
        a[k].err = err;
        a[k].locMin = locMin;
        a[k].barrier = &barrier;
        a[k].m = &m;
        a[k].globalVal = &globalVal;
        err[k] = 0;

    }

    // for(k = 0; k < p ; k++)
    // {
    //     printf("name a[%d] = %s\n",k,a[k].name);
    // }

    for (k = 1; k < p; k++) 
    {
        status = pthread_create(&tid[k], 0, thread_func, (void*) &a[k]);

        if (status != 0)
        {
                printf("main error: can't create thread, status = %d\n", status);
                pthread_mutex_destroy (&m);
                pthread_barrier_destroy(&barrier);
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
                pthread_mutex_destroy (&m);
                pthread_barrier_destroy(&barrier);
                delete []locMin;
                delete []tid;
                delete []err;
                delete []a;
                return -12;
        }

    }

    pthread_mutex_destroy(&m);

    

    for(k = 0 ; k < p; k++)
        s += err[k];
    
    if(s<0)
    {
        printf("Have errors, programm stopped\n");

        for(k = 0 ; k < p; k++)
        {
            switch (err[k])
            {
            case 0:
                break;
            case -1:
                printf("file %s doesnt exist or cant be open\n",a[k].name);
                break;

            case -2:
                printf("file %s has bad content\n",a[k].name);
                break;
            
            case -17:
            printf("non initialized thread %d connected with file %s\n",a[k].k,a[k].name);
            break;
            default:
                printf("file %s has unknown error\n",a[k].name);
                break;
            }
        }
        pthread_barrier_destroy(&barrier);
        delete []locMin;
        delete []err;
        delete []a;
        delete []tid;
        return -1;
    }

    for (k = 0; k < p; k++) 
    {
        totalres += a[k].res;
        // printf("thread %d a[%d].res = %d\n", k,k, a[k].res);

    }

    printf("Total result : %d\n",totalres);

    pthread_barrier_destroy(&barrier);
    delete []locMin;
    delete []tid;
    delete []err;
    delete []a;
    return 0;
}