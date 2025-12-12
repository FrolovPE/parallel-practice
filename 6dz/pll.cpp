#include "pll.h"
#include "args.h"
#include "mytime.h"
#include <cmath>
#define EPS 1e-15

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    // int *err = a->err;
    int n = a->n;
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

    
    
    a->localn = (k == p-1 ? n/p + n%p : n/p);
    int localn = a->localn;
    int h = n/p;
    int start = k*h;
    int end = k*h + localn;

    arr=arr;
    prev0=prev0;
    next0=next0;
    prev1=prev1;
    next1=next1;
    end=end;
    start=start;
    chng=chng;
    // printf("in thread %d n = %d p = %d localn = %d start = %d end = %d\n",k,n,p,localn,start,end);
    // printf("in thread %d arr[%d] = %lf arr[%d] = %lf arr[%d] = %lf arr[%d] = %lf\n",k,start,arr[start],start+1,arr[start + 1],end - 1 , arr[end-1],end - 2, arr[end -2]);

    // static int currlen;
    // static double sum;

    // currlen=currlen;
    // sum=sum;

    // if(k == 0)
    // {
    //     currlen = 2;
    //     sum = 0;
    // }
        
    

    // pthread_barrier_wait(barrier);

    // for(int i = start; i < end ; i++)
    // {    
    //     if( i >= 2)
    //     {
    //         prev0 = arr[i - 2];
    //         prev1 = arr[i - 1];
    //         printf("thread %d prev0 = %lf prev1 = %lf arr[%d] = %lf currlen = %d sum = %lf\n",k,prev0,prev1,i,arr[i],currlen,sum);
    //         if(fabs(arr[i] - (prev0 + prev1)) < EPS )
    //         {
    //             pthread_mutex_lock(mutex);
    //             currlen++;
    //             if(currlen > 3)
    //                 sum += arr[i];
    //             else 
    //                 sum += prev0 + prev1 + arr[i];
    //             printf("thread %d CHANGED currlen = %d sum = %lf\n",k,currlen,sum);
    //             if(i == n-1)
    //             {
    //                 if(sum > 0 || currlen > 2)
    //                 {
    //                     for(int j = 0 ; j < currlen; j++)
    //                     {
    //                         arr[i - j] = sum/(double)currlen;
    //                     }
    //                 }
    //             }

    //             pthread_mutex_unlock(mutex);
    //         }
    //         else if(fabs(arr[i] - (prev0 + prev1)) > EPS )
    //         {
    //             pthread_mutex_lock(mutex);
    //             if(sum > 0 || currlen > 2)
    //             {
    //                 for(int j = 1 ; j <= currlen; j++)
    //                 {
    //                     arr[i - j] = sum/(double)currlen;
    //                 }
    //             }
    //             currlen = 2;
    //             sum = 0;
    //             pthread_mutex_unlock(mutex);
    //         }
    //     }
    // }

    int i = (start > 1 ? start:2);

    while(i < end && i < n)
    {
        pthread_mutex_lock(mutex);
        if(fabs(arr[i] - (arr[i - 1] + arr[i - 2])) < EPS)
        {
            
            int l,r;

            l = i -2;
            r = i;

            while( l > 0 && fabs(arr[l+1] - (arr[l] + arr[l - 1])) < EPS )
            {
                l--;
            }
            while( r < n - 1 && fabs(arr[r+1] - (arr[r] + arr[r - 1])) < EPS )
            {
                // printf("before change r = %d\n",r);
                r++;
                // printf("after change r = %d\n",r);
            }

            // printf("thread %d i = %d shifted l = %d r = %d arr[%d] = %lf arr[%d] = %lf\n",k,i,l,r,l,arr[l],r,arr[r]);

            int currlen = r - l + 1;
            double sum = 0;

            for(int j = l; j < r + 1; j++ )
            {
                sum += arr[j];
            }

            for(int j = l; j < r + 1; j++ )
            {
                arr[j] = sum/(double)currlen;
            }
            
            i = r+1;


        }
        else
        {
            ++i;
        }
        pthread_mutex_unlock(mutex);
    }


    pthread_barrier_wait(barrier);

   


    elapsed = get_full_time() - elapsed;
    pthread_mutex_lock(mutex);
    *a->totalelapsed += elapsed;
    pthread_mutex_unlock(mutex);
    printf("CPU Time thread %d = %.2lf\n",k,elapsed);

    
    return (void*)SUCCESS;
    
}