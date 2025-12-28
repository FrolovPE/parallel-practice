#ifndef ARGS_H
#define ARGS_H

class args
{
public:

int k{};
int p{};
int res{};
long long  *found{};
unsigned long long  sum{};
unsigned long long int *global_sum{};
long long  lastprime{};
bool *find{};
int end{};
int N{};
args *all{};

int n{};

double *totalelapsed{};
pthread_mutex_t *mutex{};
pthread_barrier_t *barrier{};

args() = default;

};

#endif