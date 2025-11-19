#ifndef ARGS_H
#define ARGS_H

class args
{
public:

char *name{};
int k{};
int p{};
int res{};
int n{};
int *err{};
int *errsum{};
// double *locMin{};
args *allargs{};
args *lastarg{};
pthread_barrier_t *barrier{};

double x1{};
double x2{};
double x3{};
double x4{};

// static int maxlen{};
// int currlen{};

args() = default;

};

#endif