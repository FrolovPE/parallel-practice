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

void info(const args *a)
{
    if(a->n != 0)
    {
      printf("thread %d a->first1 = %lf a->first2 = %lf a->last_prev = %lf a->last = %lf\
        a->pref_len = %d a->pref_max = %lf \
        a->suff_len = %d a->suff_max = %lf \
        a->best_len = %d a->best_max = %lf a->l_max = %lf\n\n"
        ,a->k,a->first1,a->first2,a->last_prev,a->last,a->pref_len,a->pref_max,a->suff_len,a->suff_max,a->best_len,a->best_max,a->l_max);
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

    a->l_max = -1e64;
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
            a->l_max = max(a->l_max,curr);

            if(a->n==1)
            {
                a->first1 = curr;
                a->last = curr;
                prev2 = curr;
                c_len = 1;
                c_max =  curr;
                a->pref_len = 1;
                a->pref_max = curr;

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
                        a->pref_max = max(a->pref_max, curr);
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

  

    update(a->best_len,a->best_max,c_len,c_max);
    a->suff_len = c_len;
    a->suff_max = c_max;

    if(a->n == 0)
    {
        a->l_max = -1e64;
        a->best_len = 0;
        a->pref_len = 0;
        a->suff_len = 0;
    }
    else if(a->n == 1)
    {
        a->best_len = 1;
        a->pref_len = 1;
        a->suff_len = 1;
        a->best_max = a->first1;
        a->pref_max = a->first1;
        a->suff_max = a->first1;
    }
    
    // info(a);



    pthread_barrier_wait(barrier);




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
        else
        {
            *a->global_best_len = 0;
            *a->global_best_max = -1e64;

            int total{};
            int cur_len = 0;
            double cur_max = -1e64;

            double last2{};
            double last1{};

            (void)cur_len;
            (void)cur_max;
            (void)last2;
            (void)last1;

            for(int i = 0 ; i < p; i++)
            {
                
                args &arg = allargs[i];

                if(arg.n == 0) continue;

                update(*a->global_best_len,*a->global_best_max,arg.best_len,arg.best_max);

                if (total == 1)
                {
                    
                    update(*a->global_best_len, *a->global_best_max, 2, max(last1, arg.first1));

                    if (arg.n >= 2 && fabs(arg.first2 - (arg.first1 + last1))<EPS)
                    {
                        
                        int cand_len = 1 + arg.pref_len; 
                        double cand_max = max(last1, arg.pref_max);
                        update(*a->global_best_len, *a->global_best_max, cand_len, cand_max);
                    }
                }
                else if (total >= 2)
                {
                    if (fabs(arg.first1 - (last2 + last1))<EPS)
                    {
                        
                        update(*a->global_best_len, *a->global_best_max, cur_len + 1, max(cur_max, arg.first1));

                        if (arg.n >= 2 && fabs(arg.first2 - (arg.first1 + last1))<EPS)
                        {
                            
                            int cand_len = cur_len + arg.pref_len;
                            double cand_max = max(cur_max, arg.pref_max);
                            update(*a->global_best_len, *a->global_best_max, cand_len, cand_max);
                        }
                    }
                }

                if(total == 0)
                {
                    total = arg.n;

                    if(arg.n == 1)
                    {
                        cur_len = 1;
                        cur_max = arg.first1;
                        last1 = arg.first1;
                    }
                    else
                    {
                        cur_len = arg.suff_len;
                        cur_max = arg.suff_max;
                        last2 = arg.last_prev;
                        last1 = arg.last;
                    }
                //     printf("thread %d cur_len = %d cur_max = %lf last2 = %lf last1 = %lf\n",
                // arg.k,cur_len,cur_max,last2,last1);
                    continue;
                }

                if(total == 1)
                {

                    if(arg.n == 1)
                    {
                        cur_len = 2;
                        cur_max = arg.first1;

                        last2 = last1;
                        last1 = arg.first1;
                        total++;
                    }
                    else
                    {
                        if( (fabs(arg.first2 - (arg.first1 + last1)) < EPS)  && (arg.n<=2 || arg.pref_len == arg.n))//back
                        {
                            cur_len = 1 + arg.n;
                            cur_max = arg.pref_max;
                        }
                        else
                        {
                            cur_len = arg.suff_len; //esli ne prodoljilas' to perehodim k koncu etogo file
                            cur_max = arg.suff_max;
                        }

                    last2 = arg.last_prev;
                    last1 = arg.last;
                    total += arg.n;

                    }
                continue;
                }

                bool cond1 = fabs(arg.first1 - (last1 + last2)) < EPS;
                bool cond2 = (arg.n >= 2) ? fabs(arg.first2 - (arg.first1 + last1))<EPS : true;
                bool cond3 = (arg.n <= 2) ? true : (arg.pref_len == arg.n); //esli ves' file fibo posled
                (void)cond1;
                (void)cond2;
                (void)cond3;

                if(cond1 && cond2 && cond3)
                {
                    cur_len += arg.n;
                    cur_max = max(cur_max, arg.l_max);
                }
                else if(arg.n == 1)
                {
                    cur_len = 2;
                    cur_max = max(last1,arg.first1);
                }
                else
                {
                    cur_len = arg.suff_len;
                    cur_max = arg.suff_max;
                }

                if(arg.n == 1)
                {
                    last2 = last1;
                    last1 = arg.first1;
                }
                else
                {
                    last2 = arg.last_prev;
                    last1 = arg.last;
                }
                total +=arg.n;

        
            }

        }
    }

    pthread_barrier_wait(barrier);

    // printf("THREAD %d HERE\n",a->k);

    // if(a->k == 0) printf("\n\nglobal_best_len = %d global_best_max  = %lf\n",*a->global_best_len,*a->global_best_max);

    if(*a->errsum < 0) return (void*)SUCCESS;

    

    f = fopen(name,"r");
    if(!f)
    {
        return (void*)-1;
    }

    for(int i = 0; i < a->n ; i++)
    {
        fscanf(f,"%lf",&curr);

        if(curr > *a->global_best_max) res++;
    }


    a->res = res;

    // fclose(f);

   
    
    return (void*)SUCCESS;
    
}