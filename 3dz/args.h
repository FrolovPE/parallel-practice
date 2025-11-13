class args
{
public:

char *name{};
int k{};
int p{};
int res{};
int *err{};
double **allarrays{};
pthread_barrier_t *barrier{};
pthread_mutex_t *m{};
double *locMin{};
double *globalVal{};

args() = default;

};