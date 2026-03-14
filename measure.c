#include "measure.h"
#include <stdlib.h>

int rand_return_x0(State *state, double *u_return)
{
    double u = ((double)rand())/(RAND_MAX+1);

    *u_return = u;

    double sum = 0;

    for(int i = 0; i < state->amps.n; i++)
    {
        double campmod = cabs(state->amps.arr[i].amplitude);
        sum += campmod*campmod;

        if(sum >= u)
        {
            return state->amps.arr[i].idx;
        }
    }

    return -1;
}