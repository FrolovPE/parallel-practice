#ifndef ARGS_H
#define ARGS_H

class args
{
public:

double *arr{};
// double *carr{};
int k{};
int p{};
int res{};
// int *err{};
// int *errsum{};
int localn1{};
int localn2{};
int n{};
int n1{};
int n2{};
double *totalelapsed{};
pthread_mutex_t *mutex{};
pthread_barrier_t *barrier{};

args() = default;

};

#endif