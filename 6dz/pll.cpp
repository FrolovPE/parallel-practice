#include "pll.h"
#include "args.h"
#include "mytime.h"
#include <cmath>
#define EPS 1e-15


void printm(double *a, int n1, int n2)
{
    if(!a)
    {
        printf("bad pointer on matrix\n");
        return;
    }

    for(int i{}; i < n1; i++)
    {
        printf("\n");
        for(int j{}; j < n2 ; j++)
        {
            printf("%.10e ",a[i * n2 + j]);
        }
    }
    printf("\n");
}

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    // int *err = a->err;
    double *ch = a->ch;
    int n = a->n;
    int n1 = a->n1;
    int n2 = a->n2;
    int k = a->k;
    int p = a->p;
    pthread_barrier_t *barrier = a->barrier;
    pthread_mutex_t *mutex = a->mutex;
    int res = 0;
    res=res;
    a->res = res;
    double *arr = a->arr;
    // double *carr = a->carr;
    double elapsed = get_full_time();
    double prev0{},next0{},chng{},prev1{},next1{};
    double x1{},x2{},x3{},x4{};
    (void)x1;
    (void)x2;
    (void)x3;
    (void)x4;

    int z = n2-2;
    
    a->localn1 = (k == p-1 ? n1/p + n1%p : n1/p);
    a->localn2 = (k == p-1 ? n2/p + n2%p : n2/p);
    int localn1 = a->localn1;
    int localn2 = a->localn2;
    int h = n2/p;
    int start = (k == 0 ? 1:k*h);
    int end = (k == p-1 ? n2 - 1:k*h + localn2);

    n1=n1;
    n2=n2;
    arr=arr;
    prev0=prev0;
    next0=next0;
    prev1=prev1;
    next1=next1;
    n=n;
    end=end;
    start=start;
    localn1=localn1;
    localn2=localn2;
    chng=chng;
    barrier=barrier;
    ch=ch;
    z=z;
    // printf("in thread %d n1 = %d n2 = %d p = %d  start = %d end = %d\n",k,n1,n2,p,start,end);
    // printf("in thread %d arr[%d] = %lf arr[%d] = %lf arr[%d] = %lf arr[%d] = %lf\n",k,start,arr[start],start+1,arr[start + 1],end - 1 , arr[end-1],end - 2, arr[end -2]);
    // static int check;
    int zone{};
    double *buf{};
    int loc_ch{};

    // if(k == 0) check = 0;

    pthread_barrier_wait(barrier);

    for(int i = 1 ; i < n1 - 1; i++)
    {
        zone = i%3;
        buf = ch + zone * z;

        for(int j = start; j < end; j++)
        {
            double aij = arr[i * n2 + j];
            double newval = 0;

            if (aij < 0.0) 
            {
                double up    = arr[(i - 1) * n2 + j];
                double down  = arr[(i + 1) * n2 + j];
                double left  = arr[i * n2 + (j - 1)];
                double right = arr[i * n2 + (j + 1)];

                newval = up + down + left + right - 4.0 * aij;
                newval = newval;
                
                // printf("changed arr[%d,%d] = %lf =  %lf + %lf + %lf + %lf - %lf, arr[%d,%d] = %lf,arr[%d,%d] = %lf,arr[%d,%d] = %lf,arr[%d,%d] = %lf\n",i,j,newval,up,down,left,right,4 * arr[i * n2 + j],i-1,j,up,i+1,j,down,i,j-1,left,i,j+1,right);

                loc_ch++;
            }
            buf[j - 1] = newval;
            // printf("thread = %d buf[%d] = %lf\n",k,j -1, newval);
            // printf("thread %d i = %d zone = %d ch[%d * %d + %d] = %lf\n",k,i,zone,zone,z,j-1,ch[zone*z + j - 1]);
        }

        pthread_barrier_wait(barrier);


        if(i > 2)
        {
            int row1 = i - 2;
            int row2 = i -1;
            int prev1 = row1%3;
            int prev2 = row2%3;

            double *prev1buf = ch +  prev1 * z;
            double *prev2buf = ch +  prev2 * z;

            for(int s = i - 2; s < i -1;s++)
                for(int j = start; j < end ; j++)
                {
                    if(arr[s * n2 + j] < 0)
                    {
                        if(s == row1) arr[s * n2 + j] = prev1buf[j - 1];
                        if(s == row2) arr[s * n2 + j] = prev2buf[j - 1];
                    }
                }
        }

        pthread_barrier_wait(barrier);


    }




    

    // pthread_barrier_wait(barrier);
    // if(k == 0)
    // {
    //     for(int i = 0 ; i < 3 * z; i++)
    //     {
    //         if(i == 0) printf("ch:\n");
    //         printf("%lf ",ch[i]);
    //         if(i == 3 * z - 1) printf("\n");
    //     }
    // }
   
   


    elapsed = get_full_time() - elapsed;
    pthread_mutex_lock(mutex);
    *a->totalelapsed += elapsed;
    pthread_mutex_unlock(mutex);
    printf("CPU Time thread %d = %.2lf\n",k,elapsed);

    
    return (void*)SUCCESS;
    
}