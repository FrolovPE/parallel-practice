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
    int n1{},n2{};
    char *name{};
    // double curr{};
    double totalelapsed = 0;

    if(!(argc == 5 && sscanf(argv[1],"%d",&p) == 1 && sscanf(argv[2],"%d",&n1) == 1 && sscanf(argv[3],"%d",&n2) == 1))
    {
        printf("Usage: %s <p> <n1> <n2> <file with array> \n",argv[0]);
        return 0;
    }

    if(p <= 0 || n1 <=0 || n2 <=0)
    {
        printf("Error: p or n1 or n2 <=0\n");
        return -3;
    }
    if(p > n1) p = n1;

    name = argv[4];

    // printf("p = %d n1 = %d n2 = %d name = %s\n",p,n1,n2,name);
     
    pthread_t *tid = new pthread_t[p];

    // int *err = new int[p];

    int n = n1*n2;

    double *arr = new double[n];
    // double *carr = new double[n]();

    // for(k = 0; k < p ; k++) 
    // {
    //     err[k]=0;
    // }

    FILE *f;

    f = fopen(name,"r");
    if(!f)
    {
        printf("File %s doesnt exist or cant be open\n",name);
        delete []tid;
        // delete []err;
        delete []arr;
        // delete []carr;
        return -1;

    }
    else
    {
        int c = 0;
        

        while(c < n && fscanf(f,"%lf",&arr[c]) == 1)
        {
            // carr[c] = arr[c];
            c++;
            
        }

        double tmp = 0;

        if(fscanf(f,"%lf",&tmp) == 1 || c!=n)
        {
            printf("File %s has wrong size n\n",name);
            delete []tid;
            // delete []err;
            delete []arr;
            // delete []carr;
            fclose(f);
            return -2;
        }

        if(!feof(f))
        {
            printf("File %s has bad content or wrong size n\n",name);
            delete []tid;
            // delete []err;
            delete []arr;
            // delete []carr;
            fclose(f);
            return -2;

        }

        fclose(f);


        
    }

    // if(n<5)
    // {
    //     printf("algorithm not applicable\n");
    //     delete []tid;
    //     // delete []err;
    //     delete []arr;
    //     // delete []carr;
    //     return -1;
    // }

    // for(int i = 0 ; i < n; i++)
    // {
    //     if(i == 0) printf("arr: ");
    //     printf("%lf ",carr[i]);
    //     if(i == n-1) printf("\n");
    // }
    printf("matrix a:\n");
    printm(arr,n1,n2);

    args *a = new args[p];

    pthread_barrier_t barrier;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_barrier_init(&barrier,0,p);
    

    for(k = 0 ; k < p; k++)
    {
        a[k].n = n;
        a[k].n1 = n1;
        a[k].n2 = n2;
        a[k].p = p;
        a[k].arr = arr;
        a[k].k = k;
        a[k].barrier = &barrier;
        a[k].mutex = &mutex;
        a[k].totalelapsed = &totalelapsed;
        

    }


    

    for (k = 1; k < p; k++) 
    {
        status = pthread_create(&tid[k], 0, thread_func, (void*) &a[k]);

        if (status != 0)
        {
                printf("main error: can't create thread, status = %d\n", status);
                delete []tid;
                // delete []err;
                delete []a;
                // delete []carr;
                delete []arr;
                pthread_barrier_destroy(&barrier);
                pthread_mutex_destroy(&mutex);
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
                // delete []err;
                delete []a;
                // delete []carr;
                delete []arr;
                pthread_barrier_destroy(&barrier);
                pthread_mutex_destroy(&mutex);
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

    printf("RESULT %d:\n ",p);

    printm(arr,n1,n2);
            
    
    delete []arr;
    delete []tid;
    // delete []carr;
    delete []a;
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);
    return 0;
}