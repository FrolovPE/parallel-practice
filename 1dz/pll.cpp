#include "pll.h"
#include "args.h"

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    int *err = a->err;
    int n = 0; int k=a->k;
    // double x;
    char *name = a->name;
    // int p = a->p;
    pthread_barrier_t *barrier = a->barrier;
    double curr=0,prev=0,next=0;

    
        FILE *f;
        f = fopen(a->name,"r");
        if(!f)
        {
            // printf("file %s doesnt exist or cant be open\n",a->name);

            err[k] = -1;

        }
        else
        {
            int res = 0;
            a->res = res;

            while(fscanf(f,"%lf",&next) == 1)
            {
                n++;

                if(n == 1)
                {
                    prev = next;
                }
                else if(n == 2)
                {
                    curr = next;
                }
                else
                    {   
                        // printf("IN FILE %s\n",name);
                        if(curr > next && curr > prev) res++;
                        // tmp = curr;
                        // printf("prev = %lf curr = %lf next = %lf\n",prev,curr,next);
                        prev = curr;
                        curr = next;
                    }
                a->res = res;
            }
            if(!feof(f))
            {
                // printf("file %s has bad content\n",a->name);

                err[k] = -2;

            }
            fclose(f);
        }
        
    
    int p = a->p;

    pthread_barrier_wait(barrier);

    if(k == 0 )
    {
        int *errsum = a->errsum;

        for(k = 0 ; k < p; k++)
            *errsum += err[k];
        
        if(*errsum<0)
        {
            printf("Have errors, programm stopped\n");

            for(k = 0 ; k < p; k++)
            {
                switch (err[k])
                {
                case 0:
                    break;
                case -1:
                    printf("File %s doesnt exist or cant be open\n",a[k].name);
                    return (int*)-1;
                    break;

                case -2:
                    printf("File %s has bad content\n",a[k].name);
                    return (int*)-2;
                    break;

                default:
                    printf("File %s has unknown error\n",a[k].name);
                    return (int*)-17;
                    break;
                }
            }
            
            
        }
    }

   
    
        return (void*)SUCCESS;
    
}