#include "state.h"

#define TYPE Amp
#define NAME Amp_Vec
#include "vector/array_def.h"

void init_state(State *state, int n, int N)
{
    state->N = N;
    state->n = n;

    Amp_Vec_init(&state->amps);
}

void get_start_amp(State *state)
{
    Amp temp = {0};

    temp.amplitude = 1;
    temp.idx = 0;

    Amp_Vec_push(&state->amps,temp);
}