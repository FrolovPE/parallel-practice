#include "pll.h"
#include "args.h"
#include <cmath>
#define EPS 1e-15
using namespace std;

void update(int &global_len, double &global_max, int c_len, double c_max)
{
    if(c_len > global_len || (c_len == global_len && c_max > global_max ))
    {
        global_len = c_len;
        global_max = c_max;
    }
}

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    int *err = a->err;
    int k=a->k;
    int p = a->p;
    char *name = a->name;
    pthread_barrier_t *barrier = a->barrier;
    double curr=0,prev1=0,prev2=0;
    int res = 0;
    args *allargs = a->allargs;
    int c_len{};
    double c_max = -1e64;
    bool is_pref = true;

    a->seg_max = -1e64;
    a->best_len = 0;
    a->best_max = -1e64;
    a->pref_len = 0;
    a->pref_max = -1e64;
    int status{};

    (void)allargs;
    (void)is_pref;
    



    
    FILE *f;
    f = fopen(name,"r");
    if(!f)
    {
        // printf("file %s doesnt exist or cant be open\n",a->name);

        err[k] = -1;


    }
    else
    {
        while((status = (fscanf(f,"%lf",&curr) == 1)))
        {
            a->n++;
            a->seg_max = max(a->seg_max,curr);

            if(a->n==1)
            {
                a->first1 = curr;
                a->last = curr;
                prev2 = curr;
                c_len = 1;
                c_max =  curr;
                a->pref_len = 1;
                a->pref_max = 1;

            }
            else if(a->n==2)
            {
                a->first2 =  curr;
                a->last_prev = a->first1;
                a->last = curr;
                prev1 = curr;
                c_len = 2;
                c_max = max(c_max,curr);
                a->pref_len = 2;
                a->pref_max = c_max;

                update(a->best_len,a->best_max,c_len,c_max);
            }
            else
            {
                if(fabs(curr - (prev1 + prev2)) < EPS)
                {
                    c_len++;
                    c_max = max(c_max,curr);

                }
                else
                {
                    update(a->best_len,a->best_max,c_len,c_max);

                    c_len = 2;
                    c_max = max(curr,prev1);
                }

                if (is_pref)
                {
                    if (fabs(curr - (prev1 + prev2)) < EPS)
                    {
                        a->pref_len++;
                        a->pref_max = std::max(a->pref_max, curr);
                    }
                    else
                    {
                        is_pref = false;
                    }
                }

                prev2 = prev1;
                prev1 = curr;
                a->last_prev = prev2;
                a->last = prev1;

            }
        }
        if(!feof(f))
        {
            err[k] = -1;
        }

        fclose(f);
    }















    int *errsum = a->errsum;

    if(k == 0 )
    {
        

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
        // else
        // {
        //     double glval{};
        //     int glmlen{};
        //     glval=glval;
        //     glmlen=glmlen;

        //     for(int i = 0; i < p; i++)
        //     {
        //         if(allargs[i].lastarg)
        //         {
        //             if(allargs[i].lastarg->n > 1 )
        //             {
        //                 prev1 = allargs[i].lastarg->x3;
        //                 prev2 = allargs[i].lastarg->x4;

        //                 if(fabs(allargs[i].x1 -(prev1 + prev2)) < EPS)
        //                 {

        //                 }
        //             }
        //         }
        //     }
        // }
        // else
        // {
        //     int index = 0;

        //     for(int i = 0; i < p; i++)
        //     {
        //         if(allargs[i].n != 0)
        //         {
        //             index = i;
        //             // printf("index = %d allargs[i].n = %d in file %s\n",index,allargs[i].n,allargs[i].name);
        //             break;
        //         }
        //     }
            

        //    for(int i = 0; i < p ; i++)
        //    {
        //     f = fopen(allargs[i].name,"r");

        //         if(allargs[i].n != 0)
        //         {

        //             if(i!=index && i!=0)
        //             {
        //                 for(int j = i-1 ; j >= 0; j--)
        //                 {
        //                     if(allargs[j].n != 0)
        //                         {
        //                             allargs[i].lastarg = &allargs[j];
        //                             break;
        //                         }
        //                     // else
        //                     // {
        //                     //     allargs[i].lastarg = &allargs[i];
        //                     // }
        //                 }
        //                 // printf("lastarg name %s in file %s\n",allargs[i].lastarg->name,allargs[i].name);
        //             }else
        //             {
        //                 for(int j = 0 ; j < allargs[i].n; j++ )
        //                 {
        //                     fscanf(f,"%lf",&curr);
                            
        //                     if(j == 1)
        //                     {
        //                         prev1 = curr;
        //                         // *x1 = curr;
        //                     }
        //                     else if(j == 2)
        //                     {
        //                         prev2 = curr;
        //                         // *x2 = curr;
        //                     }
        //                     else
        //                     {
        //                         if(fabs(curr - (prev1 + prev2)) < eps) 
        //                         {   
                                    
        //                             currlen++;
        //                             if (maxlen < currlen) maxval = std::max(maxval,curr);
        //                         }
        //                         else
        //                         {
        //                             // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
                                    
        //                             maxlen = std::max(currlen,maxlen);
        //                             currlen = 2;
        //                         }
        //                         // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
        //                         // printf("In file %s x1 = %lf x2 = %lf x3 = %lf x4 = %lf n = %d maxlen = %d maxval = %lf \n",allargs[i].name,allargs[i].x1,allargs[i].x2,allargs[i].x3,allargs[i].x4,allargs[i].n,maxlen,maxval);

        //                         prev1 = prev2;
        //                         prev2 = curr;
        //                     }
        //                 }
                        
        //                 // printf("lastarg name %s in file %s\n",allargs[i].lastarg->name,allargs[i].name);
        //             }

        //             if(allargs[i].lastarg)
        //             {
        //                 for(int j = 0 ; j < allargs[i].n; j++ )
        //                 {
        //                     if(allargs[i].lastarg->lastarg && allargs[i].lastarg->n == 1)
        //                     {
        //                         prev2 = allargs[i].lastarg->x3;

        //                         if( allargs[i].lastarg->lastarg->n == 1)
        //                         {
        //                             prev1 = allargs[i].lastarg->lastarg->x3;

        //                             fscanf(f,"%lf",&curr);

        //                             // printf("SIT11 prev1 = %lf from file %s prev2 = %lf from file %s curr = %lf from file %s currlen = %d\n",prev1,allargs[i].lastarg->lastarg->name,prev2,allargs[i].lastarg->name,curr,allargs[i].name,currlen);


        //                             if(fabs(curr - (prev1 + prev2)) < eps) 
        //                             {   
                                        
        //                                 currlen++;
        //                                 if (maxlen < currlen) maxval = std::max(maxval,curr);
        //                             }
        //                             else
        //                             {
        //                                 // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
                                        
        //                                 maxlen = std::max(currlen,maxlen);
        //                                 currlen = 2;
        //                             }
        //                             prev1 = prev2;
        //                             prev2 = curr;
        //                         }
        //                         else if(allargs[i].lastarg->lastarg->n > 1)
        //                         {
        //                             prev1 = allargs[i].lastarg->lastarg->x4;

        //                             fscanf(f,"%lf",&curr);

        //                             // printf("SIT21 prev1 = %lf from file %s prev2 = %lf from file %s curr = %lf from file %s currlen = %d\n",prev1,allargs[i].lastarg->lastarg->name,prev2,allargs[i].lastarg->name,curr,allargs[i].name,currlen);

        //                             if(fabs(curr - (prev1 + prev2)) < eps) 
        //                             {   
                                        
        //                                 currlen++;
        //                                 if (maxlen < currlen) maxval = std::max(maxval,curr);
        //                             }
        //                             else
        //                             {
        //                                 // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
                                        
        //                                 maxlen = std::max(currlen,maxlen);
        //                                 currlen = 2;
        //                             }
        //                             prev1 = prev2;
        //                             prev2 = curr;
        //                         }
        //                         // printf("when lastarg->n == 1 maxlen = %d maxval = %lf in file %s x1 = %lf x2 = %lf x3 = %lf x4 = %lf prev1 = %lf prev2 = %lf curr = %lf currlen = %d\n",maxlen,maxval,allargs[i].name,allargs[i].x1,allargs[i].x2,allargs[i].x3,allargs[i].x4,prev1,prev2,curr,currlen);
        //                     }
        //                     else if(allargs[i].lastarg->n > 1)
        //                     {
        //                         prev1 = allargs[i].lastarg->x3;
        //                         prev2 = allargs[i].lastarg->x4;
        //                         fscanf(f,"%lf",&curr);
                            
        //                         // printf("SIT2 prev1 = %lf from file %s prev2 = %lf from file %s curr = %lf from file %s currlen = %d\n",prev1,allargs[i].lastarg->name,prev2,allargs[i].lastarg->name,curr,allargs[i].name,currlen);
                            
                            
        //                         if(fabs(curr - (prev1 + prev2)) < eps) 
        //                         {   
                                    
        //                             currlen++;
        //                             if (maxlen < currlen) maxval = std::max(maxval,curr);
        //                         }
        //                         else
        //                         {
        //                             // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
                                    
        //                             maxlen = std::max(currlen,maxlen);
        //                             currlen = 2;
        //                         }
        //                         // printf("prev1 = %lf prev2 = %lf curr = %lf\n",prev1,prev2,curr);
        //                         // printf("In file %s x1 = %lf x2 = %lf x3 = %lf x4 = %lf n = %d maxlen = %d maxval = %lf \n",allargs[i].name,allargs[i].x1,allargs[i].x2,allargs[i].x3,allargs[i].x4,allargs[i].n,maxlen,maxval);

        //                         prev1 = prev2;
        //                         prev2 = curr;
                            
        //                     }
        //                 }
        //             }
                // }

        //     fclose(f);
        //    }
        // }

    }

    pthread_barrier_wait(barrier);

    if(*a->errsum < 0) return (void*)SUCCESS;

    // // printf("globalMin  = %lf\n",globalMin);

    // f = fopen(name,"r");

    // for(int i = 0; i < n ; i++)
    // {
    //     fscanf(f,"%lf",&curr);

    //     if(curr > maxval) res++;
    // }


    a->res = res;

    // fclose(f);

   
    
    return (void*)SUCCESS;
    
}