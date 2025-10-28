#include <iostream>
#include <pthread.h>
#include "pll.h"
#include "args.cpp"


int main(int argc, char* argv[])
{
    int p = argc-1;
    int k;
    int status;

    if(argc == 1)
    {
        printf("Usage: %s <files>",argv[0]);
        return 0;
    }
     
    pthread_t *tid = new pthread_t[p];

    int *err = new int[p];

    args *a = new args[p];

    for(k = 0 ; k < p; k++)
    {
        a[k].name = argv[k+1];
        a[k].k = k;
        a[k].p = p;
        a[k].err = err;

    }

    for(k = 0; k < p ; k++)
    {
        printf("name a[%d] = %s\n",k,a[k].name);
    }

    return 0;
}