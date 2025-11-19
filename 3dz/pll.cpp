#include "pll.h"
#include "args.h"

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    int *err = a->err;
    int n = 0; int k=a->k;
    // double *locMin = a->locMin;
    int p = a->p;
    // double tmin = 0;
    char *name = a->name;
    pthread_barrier_t *barrier = a->barrier;
    double curr=0,prev1=0,prev2=0;
    int res = 0;
    args *allargs = a->allargs;
    double *x1 = &(a->x1);
    double *x2 = &(a->x2);
    double *x3 = &(a->x3);
    double *x4 = &(a->x4);

    static int maxlen = 2;
    static int currlen = 2;
    int status = 0;
    // args *lastarg = nullptr;

    a->res = res;
    // *maxlen = 2;
    // *currlen = 2;
    static double maxval = 0;

    pthread_barrier_wait(barrier);

    
        FILE *f;
        f = fopen(a->name,"r");
        if(!f)
        {
            // printf("file %s doesnt exist or cant be open\n",a->name);

            err[k] = -1;

        }
        else
        {
            

            while(status = (fscanf(f,"%lf",&curr) == 1))
            {
                n++;
                // printf("IN FILE %s\n",name);
                if(n == 1)
                {
                    prev1 = curr;
                    *x1 = curr;
                }
                else if(n == 2)
                {
                    prev2 = curr;
                    *x2 = curr;
                }
                else
                {
                    // if(curr == prev1 + prev2) *currlen++;
                    // else
                    // {
                    //     maxval = prev2;
                    //     *maxlen = std::max(*currlen,*maxlen);
                        // *currlen = 2;
                    // }
                    // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
                    prev1 = prev2;
                    prev2 = curr;
                }
            }
            if(!feof(f))
            {
                // printf("file %s has bad content\n",a->name);

                err[k] = -2;

            }
            else
            {
                *x3 = prev1;
                *x4 = prev2;
            }

            a->n = n;

            pthread_barrier_wait(barrier);

            // printf("In file %s x1 = %lf x2 = %lf x3 = %lf x4 = %lf n = %d maxlen = %d maxval = %lf \n",name,*x1,*x2,*x3,*x4,a->n,maxlen,maxval);

            
            

            fclose(f);

            // printf("tmin in file %s = %lf\n",name,tmin);
            // locMin[k] = tmin;
            
        }

    
    
    

    pthread_barrier_wait(barrier);

   


    // for(int i =0 ; i < p; i++)
    // {
    //     printf("locMin[%d] = %lf\n",i,locMin[i]);
    // }

    // static double globalMin = locMin[0];

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
           for(int i = 0; i < p ; i++)
           {
            f = fopen(allargs[i].name,"r");

                if(allargs[i].n != 0)
                {
                    if(!allargs[i].lastarg)
                    {
                        for(int j = 0 ; j < allargs[i].n; j++ )
                        {
                            fscanf(f,"%lf",&curr);
                            
                            if(j == 1)
                            {
                                prev1 = curr;
                                // *x1 = curr;
                            }
                            else if(j == 2)
                            {
                                prev2 = curr;
                                // *x2 = curr;
                            }
                            else
                            {
                                if(curr == prev1 + prev2) 
                                {   
                                    
                                    currlen++;
                                    if (maxlen < currlen) maxval = std::max(maxval,curr);
                                }
                                else
                                {
                                    // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
                                    
                                    maxlen = std::max(currlen,maxlen);
                                    currlen = 2;
                                }
                                // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
                                printf("In file %s x1 = %lf x2 = %lf x3 = %lf x4 = %lf n = %d maxlen = %d maxval = %lf \n",allargs[i].name,allargs[i].x1,allargs[i].x2,allargs[i].x3,allargs[i].x4,allargs[i].n,maxlen,maxval);

                                prev1 = prev2;
                                prev2 = curr;
                            }
                        }
                        allargs[i].lastarg = &allargs[i];
                    }
                    else
                    {
                        for(int j = 0 ; j < allargs[i].n; j++ )
                        {
                            if(allargs[i].lastarg->n == 1)
                            {
                                prev2 = allargs[i].lastarg->x4;

                                if(allargs[i].lastarg->lastarg)
                                {
                                    prev1 = allargs[i].lastarg->lastarg->x4;

                                    fscanf(f,"%lf",&curr);

                                    if(curr == prev1 + prev2) 
                                    {   
                                        
                                        currlen++;
                                        if (maxlen < currlen) maxval = std::max(maxval,curr);
                                    }
                                    else
                                    {
                                        // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
                                        
                                        maxlen = std::max(currlen,maxlen);
                                        currlen = 2;
                                    }
                                }
                                else
                                    continue;

                            }
                        }
                    }
                }

            fclose(f);
           }
        }

    }

    pthread_barrier_wait(barrier);

    // printf("globalMin  = %lf\n",globalMin);

    // f = fopen(name,"r");

    // for(int i = 0; i < n ; i++)
    // {
    //     fscanf(f,"%lf",&curr);

    //     if(curr < globalMin/2.0) res++;
    // }


    a->res = res;

    // fclose(f);

   
    
    return (void*)SUCCESS;
    
}