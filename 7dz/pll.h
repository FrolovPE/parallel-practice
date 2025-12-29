#ifndef PLL_H
#define PLL_H



#include <iostream>
#include <pthread.h>

#define SUCCESS 0

void printm(double *a, int n1, int n2);
void* thread_func(void *ptr);
void process(unsigned long long start,unsigned long long end,unsigned long long &local_sum,unsigned long long &local_found);

#endif