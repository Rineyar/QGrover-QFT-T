typedef struct amp
{
    long int idx;
    double amplitude;
} Amp;

#define TYPE Amp
#define NAME Amp_Vec
#include "vector/array_decl.h"

typedef struct state
{
    short n;
    long int N;
    long int count_amp;
    Amp_Vec amps;
} State;

void init_state(State *state, short n, long int N);