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

    
    
    a->localn1 = (k == p-1 ? n1/p + n1%p : n1/p);
    // a->localn2 = (k == p-1 ? n1/p + n%p : n/p);
    int localn1 = a->localn1;
    int localn2 = a->localn2;
    int h = n1/p;
    int start = k*h;
    int end = k*h + localn1;

    n1=n1;
    n2=n2;
    arr=arr;
    prev0=prev0;
    next0=next0;
    prev1=prev1;
    next1=next1;
    n=n;
    // end=end;
    // start=start;
    localn1=localn1;
    localn2=localn2;
    chng=chng;
    barrier=barrier;
    printf("in thread %d n1 = %d n2 = %d p = %d  start = %d end = %d\n",k,n1,n2,p,start,end);
    // printf("in thread %d arr[%d] = %lf arr[%d] = %lf arr[%d] = %lf arr[%d] = %lf\n",k,start,arr[start],start+1,arr[start + 1],end - 1 , arr[end-1],end - 2, arr[end -2]);


    for (int i = start; i < end; ++i) {

        if (i == 0 || i == n1 - 1) continue;

        for (int j = 0; j < n2; ++j) {
            if (j == 0 || j == n2 - 1) continue;

            pthread_mutex_lock(mutex);

            double aij = arr[i * n2 + j];
            if (aij < 0.0) {
                double up    = arr[(i - 1) * n2 + j];
                double down  = arr[(i + 1) * n2 + j];
                double left  = arr[i * n2 + (j - 1)];
                double right = arr[i * n2 + (j + 1)];

                double newval = up + down + left + right - 4.0 * aij;
                
                printf("changed arr[%d,%d] = %lf =  %lf + %lf + %lf + %lf - %lf, arr[%d,%d] = %lf,arr[%d,%d] = %lf,arr[%d,%d] = %lf,arr[%d,%d] = %lf\n",i,j,newval,up,down,left,right,4 * arr[i * n2 + j],i-1,j,up,i+1,j,down,i,j-1,left,i,j+1,right);

                arr[i * n2 + j] = newval;

              
            }

            pthread_mutex_unlock(mutex);
        }
    }
   
   


    elapsed = get_full_time() - elapsed;
    pthread_mutex_lock(mutex);
    *a->totalelapsed += elapsed;
    pthread_mutex_unlock(mutex);
    printf("CPU Time thread %d = %.2lf\n",k,elapsed);

    
    return (void*)SUCCESS;
    
}