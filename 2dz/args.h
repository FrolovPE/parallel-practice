#ifndef ARGS_H
#define ARGS_H

class args
{
public:

char *name{};
int k{};
int p{};
int res{};
int *err{};
int *errsum{};
double *locMin{};
double *globalMin{};
pthread_barrier_t *barrier{};

};

#endif