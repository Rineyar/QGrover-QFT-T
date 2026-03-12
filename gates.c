#include "gates.h"

int oracle(State *state, int x0) //Оракул, он меняет + на - и наоборот
{
    int i = search_amp_by_idx(state,x0); //Получение индекса

    if(i == -1) //Если ошибка
    {
        return -1; //Оракул не предсказал
    } else if(i == -2)
    {
        return 1; //0 героически заменён на 0
    }

    state->amps.arr[i].amplitude = -(state->amps.arr[i].amplitude); //Поменять
    return 1; //Успех
}