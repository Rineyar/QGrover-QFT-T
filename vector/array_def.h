#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)

// initializes members of [vec] structure for empty array
void CAT(NAME,_init)(NAME *vec)
{
    vec->arr = (TYPE*)calloc(10, sizeof(TYPE));
    if(vec->arr == NULL)
    {
        printf("Memory allocation failed");
        exit(1);
    }

    vec->n = 0;
    vec->cap = 10;
}

// makes array [vec] empty and frees its array buffer [vec->arr]
// note: this function can be called many times
void CAT(NAME,_destroy)(NAME *vec)
{
    free(vec->arr);
    vec->arr = NULL;
    vec->n = 0;
    vec->cap = 0;
}

// removes the last element from array [vec]
// returns removed element
TYPE CAT(NAME,_pop)(NAME *vec)
{
    return vec->arr[--vec->n];
}

// ensures that array [vec] has enough storage for [capacity] elements
// note: address of elements surely won’t change before [vec->n] exceeds capacity
void CAT(NAME,_reserve)(NAME *vec, int capacity)
{
    if(vec->cap < capacity)
    {
        vec->arr = (TYPE*)realloc(vec->arr, (capacity) * sizeof(TYPE));
        if(vec->arr == NULL)
        {
            printf("Memory allocation failed");
            exit(1);
        }
        vec->cap = capacity;
    }
}

// adds element [value] to the end of array [vec]
// returns index of the added element
int CAT(NAME,_push)(NAME *vec, TYPE value)
{
    if(vec->n >= vec->cap)
    {
        if(vec->cap == 0)
        {
            vec->cap = 10;
        }

        CAT(NAME,_reserve)(vec, 2 * (vec->cap));
    }

    vec->arr[vec->n] = value;
    vec->n++;

    return vec->n - 1;
}

// changes number of elements in array [vec] to [newCnt]
// if the number increases, new elements get value [fill]
// if the number decreases, some elements at the end are removed
void CAT(NAME,_resize)(NAME *vec, int newCnt, TYPE fill)
{
    if(newCnt > vec->n)
    {
        CAT(NAME,_reserve)(vec, 2 * newCnt);

        for(int i = vec->n; i < newCnt; i++)
        {
            vec->arr[i] = fill;
        }

        vec->n = newCnt;

    } else {
        vec->n = newCnt;
    }

    /*
    TYPE *temp = malloc(vec->n * sizeof(TYPE));

    for(int i = 0; i < vec->n; i++)
    {
        temp[i] = vec->arr[i];
    }

    free(vec->arr);

    vec->arr = malloc(newCnt * sizeof(TYPE));

    for(int i = 0; i < newCnt; i++)
    {
        if(i < vec->n)
        {
            vec->arr[i] = temp[i];
        } else {
            vec->arr[i] = fill;
        }
    }

    vec->n = newCnt;
    vec->cap = newCnt;

    free(temp);
    */
}

// inserts elements [arr[0]], [arr[1]], [arr[2]], ..., [arr[num-1]]
// in-between elements [vec->arr[where-1]] and [vec->arr[where]]
// note: the whole array [arr] cannot be part of array [vec]
//[where] may vary from 0 to [vec->n], [num] can also be zero
void CAT(NAME,_insert)(NAME *vec, int where, TYPE *arr, int num)
{
    if(num == 0)
    {
        return;
    }

    if(vec->cap < vec->n + num)
    {
        CAT(NAME,_reserve)(vec, 2 * (vec->n+num));
    }

    memmove(vec->arr + where + num, vec->arr + where, (vec->n - where) * sizeof(TYPE));

    memcpy(vec->arr + where, arr, num * sizeof(TYPE));

    vec->n += num;

    /*
    TYPE *temp = malloc((vec->n - where) * sizeof(TYPE));

    for(int i = where; i < vec->n; i++)
    {
        temp[i - where] = vec->arr[i];
    }

    for(int i = 0; i < num; i++)
    {
        vec->arr[where + i] = arr[i];
    }

    for(int i = 0; i < vec->n - where; i++)
    {
        vec->arr[where + num + i] = temp[i];
    }

    vec->n += num;

    free(temp);
    */
}

// removes elements [vec->arr[k]] for k = [where], [where+1], ..., [where+num-1]
// all the elements starting from [where+num]-th are shifted left by [num] positions
void CAT(NAME,_erase)(NAME *vec, int where, int num)
{
    if(num == 0)
    {
        return;
    }

    memmove(vec->arr + where, vec->arr + where + num, (vec->n - where - num) * sizeof(TYPE));

    vec->n -= num;

    /*
    TYPE *temp = malloc((vec->n - where - num) * sizeof(TYPE));

    for(int i = where + num; i < vec->n; i++)
    {
        temp[i - where - num] = vec->arr[i];
    }

    for(int i = 0; i < vec->n - where - num; i++)
    {
        vec->arr[i + where] = temp[i];
    }

    vec->n -= num;
    */
}


#undef TYPE
#undef NAME
#undef CAT2
#undef CAT