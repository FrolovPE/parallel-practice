#include "pll.h"
#include "args.cpp"

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    int *err = a->err;
    int n = 0; int k;
    double x;
    double *arr;
    int p = a[k].p;
    // double curr,prev,next;

    for(k = 0; k < p ; k++)
    {
        FILE *f;
        f = fopen(a[k].name,"r");
        if(!f)
        {
            printf("file %s doesnt exist or cant be open\n",a[k].name);

            err[k] = -2;

            fclose(f);

            continue;
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
                printf("file %s has bad content\n",a[k].name);

                err[k] = -3;

                fclose(f);

                continue;
            }

            arr = new double[n];

            fclose(f);
            f = fopen(a[k].name,"r");

            for(int i = 0 ;i < n; i++)
            {
                
                if(fscanf(f,"%lf",&arr[i]) == 0)
                {
                    printf("file %s has bad content\n",a[k].name);

                    err[k] = -3;

                    fclose(f);

                    continue;
                }

                
                
            }

            printf("n = %d\n",n);

            printf("arr :\n",n);

            for(int i =0 ; i < n; i++)
            {
                printf("%lf ",arr[i]);

                if(i == n-1) printf("\n");
            }

            if(n < 3)
            {
                res = 0;
                printf("in file %s n < 3, res = %d\n",a[k].name,res);
            }
            else
            {

                for(int i =1 ; i < n-1; i++)
                {
                    if((arr[i] > arr[i-1]) && (arr[i] > arr[i+1])) res++;
                }
            

                printf("in file %s res = %d\n",a[k].name,res);
            }

            a[k].res = res;
        }
        
    }

    int s = 0;

    for(k = 0; k < p ; k++)
    {
        s+=err[k];
    }

    if(s < 0)
    {
        printf("have errors\n");
        return (int*)-1;
    }
    else
    {
        return (int*)SUCCESS;
    }
}