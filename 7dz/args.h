#ifndef ARGS_H
#define ARGS_H

class args
{
public:

int k{};
int p{};
int res{};
unsigned long long  *found{};
unsigned long long  sum{};
unsigned long long local_f{};
unsigned long long int *global_sum{};
long long  lastprime{};
bool *find{};
unsigned long long *start{};
unsigned long long  *end{};
unsigned long long int N{};
args *all{};

unsigned long long n{};

double *totalelapsed{};
pthread_mutex_t *mutex{};
pthread_mutex_t *mutex1{};
pthread_barrier_t *barrier{};

args() = default;

};

#endif