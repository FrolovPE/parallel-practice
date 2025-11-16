#include "pll.h"
#include "args.h"

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    int *err = a->err;
    int n = 0; int k=a->k;
    double *locMin = a->locMin;
    int p = a->p;
    double tmin = 0;
    char *name = a->name;
    pthread_barrier_t *barrier = a->barrier;
    double curr=0;
    int res = 0;
    args *allargs = a->allargs;

    a->res = res;

    
        FILE *f;
        f = fopen(a->name,"r");
        if(!f)
        {
            // printf("file %s doesnt exist or cant be open\n",a->name);

            err[k] = -1;

        }
        else
        {
            

            while(fscanf(f,"%lf",&curr) == 1)
            {
                n++;

                if(n>0)
                {   
                    if(n == 1) tmin = curr;
                    else
                    {
                        if(curr < tmin)
                        {
                            tmin = curr;
                        }
                    }

                }
            }
            if(!feof(f))
            {
                // printf("file %s has bad content\n",a->name);

                err[k] = -2;

            }

            fclose(f);

            // printf("tmin in file %s = %lf\n",name,tmin);
            locMin[k] = tmin;
            
        }

    
    
    

    pthread_barrier_wait(barrier);

    // for(int i =0 ; i < p; i++)
    // {
    //     printf("locMin[%d] = %lf\n",i,locMin[i]);
    // }

    static double globalMin = locMin[0];

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
                    break;

                case -2:
                    printf("File %s has bad content\n",a[k].name);
                    break;

                default:
                    printf("File %s has unknown error\n",a[k].name);
                    break;
                }
            }
            
            
        }
        else
        {
            for(int i =0 ;i < p ; i++)
            {
                if(locMin[i] < globalMin)
                {
                    globalMin = locMin[i];
                }
            }
        }
    }

    pthread_barrier_wait(barrier);

    // printf("globalMin  = %lf\n",globalMin);

    f = fopen(name,"r");

    for(int i = 0; i < n ; i++)
    {
        fscanf(f,"%lf",&curr);

        if(curr < globalMin/2.0) res++;
    }


    a->res = res;

    fclose(f);

   
    
    return (void*)SUCCESS;
    
}