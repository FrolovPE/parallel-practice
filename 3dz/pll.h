#ifndef PLL_H
#define PLL_H

#include <iostream>
#include <pthread.h>
#include "args.h"

#define SUCCESS 0

void* thread_func(void *ptr);
void info(const args *a);
void update(int &global_len, double &global_max, int c_len, double c_max);

#endif