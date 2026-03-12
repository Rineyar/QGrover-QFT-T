#include "check.h"

int get_n(State state)
{
    return state.n;
}

int get_N(State state)
{
    return state.N;
}

int get_amps_count(State state)
{
    return state.amps.n;
}

Amp get_amp(State state, int idx)
{
    return state.amps.arr[idx];
}