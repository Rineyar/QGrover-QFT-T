#include "qft.h"



static int bit_reverse_by_idx(int idx, int n) //Разворот битов индекса
{
    int rev = 0;

    for(int i = 0; i < n; i++)
    {
        rev <<= 1;
        rev |= (idx & 1);
        idx >>= 1;
    }
    
    return rev;
}

static void reverse_amps_by_bit(State *state)
{
    for(int i = 0; i < state->amps.n; i++)
    {
        int h = bit_reverse_by_idx(i,state->n);

        if(h > i)
        {
            complex double temp = state->amps.arr[i].amplitude;
            state->amps.arr[i].amplitude = state->amps.arr[h].amplitude;
            state->amps.arr[h].amplitude = temp;
        }
    }
}

static void butterfly(State *state, int left, int right, complex double w)
{
    complex double u = state->amps.arr[left].amplitude, v = state->amps.arr[right].amplitude * w;

    state->amps.arr[left].amplitude = u + v;
    state->amps.arr[right].amplitude = u - v;
}

static void linecalc(State *state, int start, int len)
{
    complex double w = 1;

    for(int i = 0; i < len/2-1; i++)
    {
        butterfly(state,start+i,start+i+len/2,w);

        w *= cpow(M_E,(2*M_PI*I/len));
    }
}

int qft(State *state)
{

}

int iqft(State *state)
{

}