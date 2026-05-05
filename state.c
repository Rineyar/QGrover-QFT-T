#include "state.h"
#define TYPE Amp
#define NAME Amp_Vec
#include "vector/array_def.h"
#include <stdlib.h>

#define EPS 1e-10

void init_state(State *state, int n, int N) //Инициализация памяти
{
    state->N = N; //Записать полученные n/N в state
    state->n = n;
    state->amps.n = 0; //Чистим амплитуды

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
    free(state);
    state = NULL;
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
        return -1;
    }

    int i = search_amp_by_idx(state,idx); //Получение индекса амплитуды

    if(is_amp_null(amp)) //Если новая амплитуда 0
    {
        if(i != -2) //И есть куда вставить
        {
            remove_amp_by_i(state,i); //Удаляем
        }

        return 0; //Закончили нулём
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

    if(i != -2) //Нашли
    {
        *return_amp = state->amps.arr[i].amplitude; //Вернули
        return 0;
    }

    // printf("[DBG] i не найден! | i - %d idx - %d\n",i,idx);
    *return_amp = 0;
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

int is_amp_null(const complex double amp) //Проверка на 0
{
    if(creal(amp)*creal(amp)+cimag(amp)*cimag(amp) < EPS*EPS) //0? //(creal(amp) == 0 && cimag(amp) == 0) //creal(amp)^2+cimag(amp)^2 < EPS^2 //cabs(amp) < EPS
    {
        return 1; //Да
    }

    return 0; //Нет
}

double rand_double(double min, double max) {
    return min + (max - min) * rand() / RAND_MAX;
}

int set_random_state(State* state)
{
    // Каждая амплитуда - рандомное число типа double в диапазоне [-1, 1]
    for(int i = 0; i < state->N; i++)
    {
        Amp_Vec_push(&state->amps, (Amp){.idx=i, .amplitude=rand_double(-1, 1)});
    }

    // Квадратный корень из суммы квадратов модулей для нормировки
    double norm_sqrt = sqrt(norm_square_amps(state));

    // Нормируем состояние
    for(int i = 0; i < state->N; i++) 
    {
        double complex amp;
        int e = read_amp_by_idx(state, i, &amp);
        set_amp_by_idx(state, amp / norm_sqrt, i);
    }
}

void print_state(State *state, const char* msg, FILE* file)
{ 
    fprintf(file, "%s\n", msg);               
    for (int i = 0; i < state->N; ++i) 
    {
        double complex amp;
        read_amp_by_idx(state, i, &amp);
        double real = creal(amp);
        double imag = cimag(amp);
        fprintf(file, "Amplitude %d: %g + %gi\n", i, real, imag);
    }
    fprintf(file, "\n");
}