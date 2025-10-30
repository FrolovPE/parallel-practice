#include "pll.h"
#include "args.h"

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    int *err = a->err;
    int n = 0; int k=a->k;
    double x;
    double *arr;
    int p = a->p;
    // double curr,prev,next;

    
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
            int res = 0;

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

            // printf("\nn = %d\n",n);

            // printf("\narr in file %s: ",a->name);

            // for(int i =0 ; i < n; i++)
            // {
            //     printf("%lf ",arr[i]);

            //     if(i == n-1) printf("\n");
            // }

            if(n < 3)
            {
                res = 0;
                // printf("in file %s n < 3, res = %d\n",a->name,res);
            }
            else
            {

                for(int i =1 ; i < n-1; i++)
                {
                    if((arr[i] > arr[i-1]) && (arr[i] > arr[i+1])) res++;
                }
            

                // printf("in file %s res = %d\n",a->name,res);
            }

            a->res = res;
        }
        
    

    // int s = 0;

    // for(k = 0; k < p ; k++)
    // {
    //     s+=err[k];
    // }

    // if(s < 0)
    // {
    //     printf("have errors\n");
    //     return (int*)-1;
    // }
    // else
    // {
        delete []arr;
        return (int*)SUCCESS;
    // }
}