#include "check.h"

short get_n(State state)
{
    return state.n;
}

long int get_N(State state)
{
    return state.N;
}

int get_amps_count(State state)
{
    return state.amps.n;
}

Amp get_amp(State state, long int idx)
{
    return state.amps.arr[(int)idx];
}