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

double first1{}, first2{};
double last_prev{}, last{};

double l_max{};

int pref_len{};       
double pref_max{};

int suff_len{};       
double suff_max{};

int best_len{};       
double best_max{};


int* global_best_len{};       
double* global_best_max{};   

// static int maxlen{};
// int currlen{};

args() = default;

};

#endif