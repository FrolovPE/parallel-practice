#include "pll.h"
#include "args.h"

void* thread_func(void *ptr)
{
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_barrier_t barrier;
    args *a = (args*) ptr;
    int *err = a->err;double *locMin = a->locMin;
    int n = 0; int k=a->k;
    double x,localMin,globalMin;
    double *arr;
    int p = a->p;
    int res = 0;
    pthread_barrier_init(&barrier,0,p);
    // double curr,prev,next;

    // pthread_mutex_lock(&m);
        FILE *f;
        f = fopen(a->name,"r");
        if(!f)
        {
            // printf("file %s doesnt exist or cant be open\n",a->name);

            err[k] = -1;

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

        }

        pthread_barrier_wait(&barrier);

        if(k == p-1){
            globalMin = locMin[0];
            for(int i = 0 ; i < p; i++)
            {
                if(locMin[i] < globalMin)
                    globalMin = locMin[i];
            }
        }

         pthread_barrier_wait(&barrier);

        for(int i =0 ; i < n; i++)
        {
            if(arr[i] < globalMin)
                res++;
        }

        a->res = res;
        
    //    pthread_mutex_unlock(&m);






        pthread_mutex_destroy(&m);
        pthread_barrier_destroy(&barrier);
        delete []arr;
        return (int*)SUCCESS;
    // }
}