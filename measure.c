#include "measure.h"
#include <stdlib.h>

int rand_return_x0(State *state/*, double *u_return*/) //Возвращает индекс наиболее вероятного
{
    double u = ((double)rand())/(RAND_MAX+1); //Шанс фозврата именно сейчас

    // *u_return = u; //В прошлом отладочная, можно убрать

    double sum = 0; //Сумма амплитуд

    for(int i = 0; i < state->amps.n; i++) //Гоняем по всем ненулевым
    {
        double campmod = cabs(state->amps.arr[i].amplitude); //Модуль комплексной амплитуды
        sum += campmod*campmod; //Добавить её вероятность в сумма

        if(sum >= u) //Если нашлось (или ложное срабатывание)
        {
            return state->amps.arr[i].idx; //Вернуть индекс
        }
    }

    return -1; //Что-то пошло не так
}