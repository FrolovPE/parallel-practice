#include <iostream>
#include <pthread.h>
#include "pll.h"
#include "args.h"


int main(int argc, char* argv[])
{
    int p = argc-1;
    int k;
    int status;
    int s = 0;
    int totalres = 0;

    if(argc == 1)
    {
        printf("Usage: %s <files>\n",argv[0]);
        return 0;
    }
     
    pthread_t *tid = new pthread_t[p];

    int *err = new int[p];

    for(k = 0; k < p ; k++) err[k]=0;

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
                return -11;

        }
    }

    for (k = 0; k < p; k++) 
    {
        status = pthread_join(tid[k],0);
        if (status != 0)
        {
                printf("error in pthread_join, status = %d\n", status);
                return -12;
        }

    }

    

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

            default:
                printf("file %s has unknown error\n",a[k].name);
                break;
            }
        }

        return -1;
    }

    for (k = 0; k < p; k++) 
    {
        totalres += a[k].res;
        // printf("thread %d a[%d].res = %d\n", k,k, a[k].res);

    }

    printf("Total result : %d\n",totalres);
          

    return 0;
}