#include "pll.h"
#include "args.h"

void* thread_func(void *ptr)
{
    
    
    args *a = (args*) ptr;
    int *err = a->err;double *locMin = a->locMin;
    int n = 0; int k=a->k;
    double x;
    // double localMin;
    double *globalMin = a->globalVal;
    int p = a->p;
    int res = 0;
    double **allarrays = a->allarrays;
    double *arr;
    char*name = a->name;
    pthread_barrier_t *barrier = a->barrier;
    pthread_mutex_t *m = a->m;
    static bool exitflag = false;
    
    // double curr,prev,next;

    pthread_mutex_lock(m);
        FILE *f;
        f = fopen(a->name,"r");
        if(!f)
        {
            printf("file %s doesnt exist or cant be open\n",a->name);
            exitflag = true;

            err[k] = -1;
            pthread_mutex_unlock(m);

            pthread_barrier_wait(barrier);
            pthread_barrier_wait(barrier);
            pthread_barrier_wait(barrier);
            pthread_barrier_wait(barrier);
            

            return (int*)-1;

        }
        else
        {
           

            while(fscanf(f,"%lf",&x) == 1)
            {
                n++;
            }
            if(!feof(f))
            {
                printf("file %s has bad content\n",a->name);

                err[k] = -2;
                exitflag = true;
                printf("exitflag  = %d in thread %d\n",exitflag,k);

                fclose(f);

                pthread_mutex_unlock(m);
                pthread_barrier_wait(barrier);
                pthread_barrier_wait(barrier);
                pthread_barrier_wait(barrier);
                pthread_barrier_wait(barrier);
                

                return (int*)-2;

            }
            pthread_mutex_unlock(m);

            pthread_barrier_wait(barrier);

            printf("exitflag  = %d in thread %d\n",exitflag,k);

            if(exitflag)
            {
                err[k] = 0;
                fclose(f);

                pthread_mutex_unlock(m);
                pthread_barrier_wait(barrier);
                pthread_barrier_wait(barrier);
                pthread_barrier_wait(barrier);
                

                return (int*)-2;
            }

            arr = new double[n+1];
            allarrays[k] = arr;

            // pthread_barrier_wait(barrier);

            // for(int i = 0 ; i < p; i++)
            // {
            //     for(int j = 0 ; j < p ; j++)
            //     {
            //         printf("%lf ",allarrays[i][j]);
            //     }
            // }

            // for(int j = 0 ; j < n ; j++)
            //     {
            //         printf("%lf ",allarrays[k][j]);
            //     }

           



            if(n <= 0 )
            {
                err[k] = 0;


                // pthread_mutex_unlock(m);
                // pthread_barrier_wait(barrier);
                // pthread_barrier_wait(barrier);
                // pthread_barrier_wait(barrier);
                
            }

            fclose(f);

            f = fopen(a->name,"r");

            for(int i = 1 ;i < n+1; i++)
            {
                
                if(fscanf(f,"%lf",&arr[i]) == 0)
                {
                    // printf("file %s has bad content\n",a->name);

                    err[k] = -2;

                    fclose(f);

                    delete []arr;

                    pthread_mutex_unlock(m);
                    pthread_barrier_wait(barrier);
                    pthread_barrier_wait(barrier);
                    pthread_barrier_wait(barrier);
                    exitflag = true;

                    return (int *)-2;

                }

                
                
            }

            fclose(f);
        }

            arr[0] = n;

            

            // for(int j = 0 ; j < n ; j++)
            // {
            //     printf("%lf ",arr[j]);
            // }
            

        //     localMin = arr[0];

        //     for(int i =0 ; i < n; i++)
        //     {
        //         if(arr[i] < localMin)
        //             localMin = arr[i];
        //     }
            
        //     locMin[k] = localMin;
        //     err[k] = 0;

        // }

        pthread_mutex_unlock(m);

        // printf("arr of file %s\n",name);
        //     for(int j = 0 ; j < n+1 ; j++)
        //         {
                    
        //             printf("%lf ",allarrays[k][j]);
        //         }

        //     printf("\n");

            pthread_barrier_wait(barrier); //must have

            

        int serialT = pthread_barrier_wait(barrier);

        static int maxlen = 0;
        static int counter = 0;
        double *marr{};

        if(k == 0)
        {   
            

            for(int i = 0 ; i < p; i++)
            {
                counter+=allarrays[i][0];
            }

            // printf("counter = %d\n",counter);

            marr = new double[counter];
            int cc=0;

            for(int i = 0 ; i < p; i++)
               {
                 for(int j = 1 ; j < allarrays[i][0]+1 ; j++)
                    {
                        marr[cc++] = allarrays[i][j];
                        // printf("marr[%d] = %lf \n",j-1,marr[j-1]);
                        
                    }
                }

            // printf("arr of whole seq %s\n",name);
            // for(int j = 0 ; j < counter ; j++)
            //     {
                    
            //         printf("%lf ",marr[j]);
            //     }
            // printf("\n");

            
            int curr = 0;

            for(int i = 2 ; i < counter ; i++)
                {
                    if(marr[i] == marr[i-1] + marr[i-2]){
                         curr++;
                        //  printf("%lf == %lf + %lf\n",marr[i],marr[i-1],marr[i-2]);
                    }
                    else
                    {
                        maxlen = std::max(curr,maxlen);
                        curr = 0;
                    }
                }

        }

        

        pthread_barrier_wait(barrier);

        // printf("maxlen = %d\n",maxlen);

        // printf("globalMin in thread %d equal %lf\n",k,*(globalMin));

        if(k == 0)
        {
            for(int i = 0 ; i < counter ; i++)
            {
                if(marr[i] > maxlen) res++;
            }

            a->res = res;
        
        }


        pthread_barrier_wait(barrier);



        
        if(k == 0 && marr) delete []marr;
        delete []arr;
        return (int*)SUCCESS;
    // }
}