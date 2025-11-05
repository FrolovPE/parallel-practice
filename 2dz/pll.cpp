#include "pll.h"
#include "args.h"

void* thread_func(void *ptr)
{
    
    
    args *a = (args*) ptr;
    int *err = a->err;double *locMin = a->locMin;
    int n = 0; int k=a->k;
    double x,localMin;
    double *arr;
    double *globalMin = a->globalVal;
    int p = a->p;
    int res = 0;
    pthread_barrier_t *barrier = a->barrier;
    pthread_mutex_t *m = a->m;
    
    // double curr,prev,next;

    pthread_mutex_lock(m);
        FILE *f;
        f = fopen(a->name,"r");
        if(!f)
        {
            // printf("file %s doesnt exist or cant be open\n",a->name);

            err[k] = -1;
            pthread_mutex_unlock(m);

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
                // printf("file %s has bad content\n",a->name);

                err[k] = -2;

                fclose(f);

                pthread_mutex_unlock(m);
                pthread_barrier_wait(barrier);
                pthread_barrier_wait(barrier);

                return (int*)-2;

            }

            arr = new double[n];

            fclose(f);

            f = fopen(a->name,"r");

            for(int i = 0 ;i < n; i++)
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

                    return (int *)-2;

                }

                
                
            }

            localMin = arr[0];

            for(int i =0 ; i < n; i++)
            {
                if(arr[i] < localMin)
                    localMin = arr[i];
            }
            
            locMin[k] = localMin;
            err[k] == 0;

        }

        pthread_mutex_unlock(m);

        int serialT = pthread_barrier_wait(barrier);

        if(serialT == PTHREAD_BARRIER_SERIAL_THREAD)
        {   
            // printf("thread %d is serial with filename %s\n",k,a->name);
            *(globalMin) = locMin[0];
            for(int i = 0 ; i < p; i++)
            {
                if(err[i] == 0 && locMin[i] < *(globalMin))
                    *(globalMin) = locMin[i];
            }
        }

        

        pthread_barrier_wait(barrier);

        // printf("globalMin in thread %d equal %lf\n",k,*(globalMin));

        for(int i =0 ; i < n; i++)
        {
            if(arr[i] < *(globalMin)/2.0 )
                res++;
        }

        a->res = res;
        
       






        
        
        delete []arr;
        return (int*)SUCCESS;
    // }
}