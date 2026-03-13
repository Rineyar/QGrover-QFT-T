#include "gates.h"
#include <math.h>

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

void set_uniform_superposition(State *state) //Суперпозиция доля всех
{
    state->amps.n = 0; //Чистим амплитуды

    Amp temp = {0}; //Времянка

    temp.amplitude = 1/sqrt(state->N); //1/sqrt(N)
    temp.idx = 0; //Индекс амплитуды

    for(int i = 0; i < state->N; i++) //Создание неразряженного состояния
    {
        temp.idx = i; //Присвоение индекса
        Amp_Vec_push(&state->amps,temp); //Добавление амплитуды
    }
}

void diffusion(State *state) //Диффузирование суперпозиций
{
    complex double mean = 0; //Среднее значение амплитуд

    for(int i = 0; i < state->amps.n; i++) //Считаем сумма
    {
        mean += state->amps.arr[i].amplitude; //Сумма
    }

    mean /= state->N; //Делим на кол-во

    for(int i = 0; i < state->amps.n; i++) //Применяем к каждому
    {
        state->amps.arr[i].amplitude = 2*mean - state->amps.arr[i].amplitude; //ai = 2a.mean - ai
    }
}