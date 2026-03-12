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

void set_start_state(State *state)
{
    Amp temp = {0};

    temp.amplitude = 1;
    temp.idx = 0;

    state->amps.n = 0;

    Amp_Vec_push(&state->amps,temp);
}

void clear_state(State *state)
{
    state->n = state->N = 0;

    Amp_Vec_destroy(&state->amps);
}

int search_amp_by_idx(State state, int idx)
{
    for(int i = 0; i < state.amps.n; i++)
    {
        if(state.amps.arr[i].idx == idx)
        {
            return i;
        }
    }

    return -1;
}