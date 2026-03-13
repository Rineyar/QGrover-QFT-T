#include "types.h"
#include "gates.h"
#include "state.h"

int grover_step(State *state, int x0); //Один шаг алгоритма Гровера

int grover_iters(State *state); //Получить кол-во итераций Гровера

int grover_alg(State *state, int x0); //Алгоритм Гровера