#ifndef ARGS_H
#define ARGS_H

class args
{
public:

double *arr{};
double *carr{};
int k{};
int p{};
int res{};
// int *err{};
int *errsum{};
int localn{};
int n{};
double *totalelapsed{};
pthread_barrier_t *barrier{};

args() = default;

};

#endif