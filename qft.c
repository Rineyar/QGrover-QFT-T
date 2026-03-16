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

static void linecalc(State *state, int start, int len, complex double wlen)
{
    complex double w = 1;

    for(int i = 0; i < len/2; i++)
    {
        butterfly(state,start+i,start+i+len/2,w);

        w *= wlen;
    }
}

static void fft_step(State *state, int len, int sign)
{
    complex double wlen = 0;

    if(sign < 0)
    {
        wlen = cexp(-2*M_PI*I/len);
    } else if(sign > 0)
    {
        wlen = cexp(2*M_PI*I/len);
    }

    for(int i = 0; i < state->N/len; i++)
    {
        linecalc(state,i*len,len,wlen);
    }
}

static void fft(State *state, int sign)
{
    int len = 2;

    while(len <= state->N)
    {
        fft_step(state,len,sign);

        len *= 2;
    }
}

static void normalize(State *state)
{
    complex double k = 1/sqrt(state->N);

    for(int i = 0; i < state->amps.n; i++)
    {
        state->amps.arr[i].amplitude *= k;
    }
}

void qft(State *state)
{
    reverse_amps_by_bit(state);

    fft(state,1);

    normalize(state);
}

void iqft(State *state)
{
    reverse_amps_by_bit(state);

    fft(state,-1);

    normalize(state);
}