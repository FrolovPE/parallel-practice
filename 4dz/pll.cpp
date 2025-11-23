#include "pll.h"
#include "args.h"
#include "mytime.h"

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    // int *err = a->err;
    int n = a->n;
    int k = a->k;
    int p = a->p;
    pthread_barrier_t *barrier = a->barrier;
    int res = 0;
    res=res;
    a->res = res;
    double *arr = a->arr;
    // double *carr = a->carr;
    double elapsed = get_full_time();
    double prev{},next{},chng{};

    
    
    a->localn = (k == p-1 ? n/p + n%p : n/p);
    int localn = a->localn;
    int h = n/p;
    int start = k*h;
    int end = k*h + localn;

    arr=arr;
    prev=prev;
    next=next;
    end=end;
    start=start;
    // printf("in thread %d n = %d p = %d localn = %d\n",k,n,p,localn);

        if(localn > 4)
        {
            for(int i = start; i < end; i++ )
            {
                // if(i - 2 >= 0 && i + 2 < n)
                if(i >= start + 2 && i <=end - 2)
                {
                    if(arr + (i - 2) >= arr && arr + (i + 2) < arr + n )
                    {   
                        prev = arr[i - 2];
                        next = arr[i + 2];
                        chng = (prev + next)/2.0;
                    }
                    printf("for a[%d] = %lf prev = %lf next = %lf chng = %lf\n",i,arr[i],prev,next,chng);
                }
                

            }
        }
        // else
        // {
        //     if(k == 0)
        //     {
        //         for(int i = start; i < end; i++ )
        //         {
                    
        //         }
        //     }
        // }
    



    pthread_barrier_wait(barrier);

   



    // if(k == 0 )
    // {
    //     int *errsum = a->errsum;

    //     for(int i  = 0 ; i < p; i++)
    //         *errsum += err[i];
        
    //     if(*errsum<0)
    //     {
    //         printf("Have errors, programm stopped\n");

    //         for(int i = 0 ; i < p; i++)
    //         {
    //             switch (err[i])
    //             {
    //             case 0:
    //                 break;
    //             case -1:
    //                 // printf("File %s doesnt exist or cant be open\n",a[k].name);
    //                 break;

    //             case -2:
    //                 // printf("File %s has bad content\n",a[k].name);
    //                 break;

    //             default:
    //                 // printf("File %s has unknown error\n",a[k].name);
    //                 break;
    //             }
    //         }
            
            
    //     }
    //     // else
    //     // {

    //     // }
    // }

    // pthread_barrier_wait(barrier);


    // f = fopen(name,"r");

    // a->res = res;

    // fclose(f);

    elapsed = get_full_time() - elapsed;
    *a->totalelapsed += elapsed;
    printf("CPU Time thread %d = %.2lf\n",k,elapsed);

    
    return (void*)SUCCESS;
    
}