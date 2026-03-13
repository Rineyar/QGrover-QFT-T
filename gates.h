#include "types.h"
#include "state.h"

int oracle(State *state, int x0); //Изменение амплитуды на отрицательную у нулевого индекса

void set_uniform_superposition(State *state); //Получение равномерной суперпозиции и добавление её амплитуды

void diffusion(State *state); //Оператор диффузии для суперпозиций