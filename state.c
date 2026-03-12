#include "state.h"
#include <time.h>
#include <complex.h>

#define TYPE Amp
#define NAME Amp_Vec
#include "vector/array_def.h"

void init_state(State *state, int n, int N) //Инициализация памяти
{
    state->N = N; //Записать полученные n/N в state
    state->n = n;

    Amp_Vec_init(&state->amps); //Инициализация вектора амплитуд
}

void set_start_state(State *state) //Установка стартовой амплитуды
{
    Amp temp = {0}; //Стартовая

    temp.amplitude = 1; //Полная
    temp.idx = 0; //Первая

    state->amps.n = 0; //Отчистка вектора

    Amp_Vec_push(&state->amps,temp); //Добавить стартовую
}

void clear_state(State *state) //Прочистить -трубы- память
{
    state->n = state->N = 0; //Убрать n/N

    Amp_Vec_destroy(&state->amps); //Чистить вектор
}

int search_amp_by_idx(const State *state, int idx) //Поиск амплитуды по индексу
{
    if(idx < 0 || idx >= state->N) //Если индекс некорректен
    {
        return -1;
    }

    for(int i = 0; i < state->amps.n; i++) //Ищем
    {
        if(state->amps.arr[i].idx == idx) //Нашли
        {
            return i; //Вернули индек в векторе
        }
    }

    return -2; //Не нашли
}

int set_amp_by_idx(State *state, complex double amp, int idx) //Установить новую амплитуду
{
    if(idx < 0 || idx >= state->N) //Если индекс некорректен
    {
        return 0;
    }

    int i = search_amp_by_idx(state,idx); //Получение индекса амплитуды

    if(creal(amp) == 0 && cimag(amp) == 0) //Если новая амплитуда 0
    {
        if(i != -2) //И есть куда вставить
        {
            remove_amp_by_i(state,i); //Удаляем
        }

        return 3; //Закончили нулём
    }

    if(i != -2) //Если такая есть
    {
        state->amps.arr[i].amplitude = amp; //Заменить

        return 1; //Успешно изменена
    } //Не нашли

    Amp temp = {0}; //Времянка

    temp.amplitude = amp; //Установить амплитуду и индекс
    temp.idx = idx;

    Amp_Vec_push(&state->amps,temp); //Добавить

    return 2; //Добавлена новая
}

int read_amp_by_idx(const State *state, int idx, complex double *return_amp)
{
    if(idx < 0 || idx >= state->N) //Если индекс некорректен
    {
        return -1;
    }

    int i = search_amp_by_idx(state,idx); //Индекс амплитуды

    if(i != 2) //Нашли
    {
        *return_amp = state->amps.arr[i].amplitude; //Вернули
        return 1;
    }

    return 0; //Не нашли, значит 0
}

int remove_amp_by_i(State *state, int i)
{
    if(i < 0 || i >= state->amps.n) //Некорретный индекс
    {
        return -1;
    }

    if(state->amps.n == 1) //Если элемент единственный
    {
        state->amps.n = 0; //Чистка
        return 2; //Возврат кода
    } //Если не единственный

    state->amps.arr[i] = state->amps.arr[state->amps.n-1]; //Конечный поставить в пустой слот
    state->amps.n--; //Размер-

    return 1;
}