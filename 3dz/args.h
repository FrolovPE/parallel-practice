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
args *allargs{};
pthread_barrier_t *barrier{};

int x1{};
int x2{};
int x3{};
int x4{};

int maxlen{};
int currlen{};

args() = default;

};

#endif