#include "pll.h"
#include "args.h"
#include "mytime.h"

void* thread_func(void *ptr)
{
    args *a = (args*) ptr;
    // int *err = a->err;
    int n = a->n;
    int k = a->k;
    int p = a->p;
    pthread_barrier_t *barrier = a->barrier;
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
    end=end;
    start=start;
    chng=chng;
    printf("in thread %d n = %d p = %d localn = %d start = %d end = %d\n",k,n,p,localn,start,end);
    printf("in thread %d arr[%d] = %lf arr[%d] = %lf arr[%d] = %lf arr[%d] = %lf\n",k,start,arr[start],start+1,arr[start + 1],end - 1 , arr[end-1],end - 2, arr[end -2]);



    if(arr + start >= arr + 2 && arr + start < arr + (n - 2))
    {
        // printf("in thread %d start arr[%d] = %lf ",k,start,arr[start]);
        x1 = (arr[start - 2] + arr[start + 2])/2.0;
        if(localn > 1) 
        {
            x2 = (arr[start + 1 - 2] + arr[start+ 1 + 2])/2.0;
            // printf(" start + 1 arr[%d] = %lf ",start+1,arr[start + 1]);
        }
        // printf("\n");
    }
    if(arr + end <= arr + (n - 2) && arr + end > arr + 2)
    {
        // printf("in thread %d end - 1 arr[%d] = %lf ",k,end-1,arr[end-1]);
        x4 = (arr[end -1 - 2] + arr[end -1 + 2])/2.0;
        if(localn > 1) 
        {
            x3 = (arr[end - 2 - 2] + arr[end - 2 + 2])/2.0;
            // printf(" end - 2 arr[%d] = %lf ",end - 2,arr[end-2]);
        }
        // printf("\n");
    }
    
    if(localn > 4)
    {    
        int c0 = 0;
        int c1 = 0;
        for(int i = start; i < end ; i++)
        {
            if(i >= start + 2 && i <end - 2 && i%2 == 0)
            {
                

                if(c0 > 0)
                {
                    next0 = arr[i];
                    arr[i] = (prev0 + arr[i + 2])/2.0;
                    // printf("for a[%d] = %lf prev = %lf next = %lf \n",i,arr[i],prev0,next0);
                    prev0 = next0;
                }
                else
                {
                prev0 = arr[i];
                arr[i] = (arr[i - 2] + arr[i + 2])/2.0;
                
                // printf("for a[%d] = %lf prev = %lf\n",i,arr[i],prev0);
                c0++;
                }
            }
            else if(i >= start + 2 && i <end - 2 && i%2 == 1)
            {
                

                if(c1 > 0)
                {
                    next1 = arr[i];
                    arr[i] = (prev1 + arr[i + 2])/2.0;
                    // printf("for a[%d] = %lf prev = %lf next = %lf \n",i,arr[i],prev1,next1);
                    prev1 = next1;
                }
                else
                {
                prev1 = arr[i];
                arr[i] = (arr[i - 2] + arr[i + 2])/2.0;
                
                // printf("for a[%d] = %lf prev = %lf\n",i,arr[i],prev1);
                c1++;
                }
            }

            
        }
    }
    


    

    pthread_barrier_wait(barrier);

    if(arr + start >= arr + 2 && arr + start < arr + (n - 2) )
    {
        arr[start] = x1;
        if(localn > 1 && arr + start + 1 < arr + (n - 2) ) arr[start + 1] = x2;
    }
    if(arr + end <= arr + (n - 2) && arr + end > arr + 2 )
    {
        arr[end - 1] = x4;
        if(localn > 1 && arr + end -1 > arr + 2) arr[end - 2] = x3;
    }


    pthread_barrier_wait(barrier);

   


    elapsed = get_full_time() - elapsed;
    *a->totalelapsed += elapsed;
    printf("CPU Time thread %d = %.2lf\n",k,elapsed);

    
    return (void*)SUCCESS;
    
}