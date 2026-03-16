#include "qft.h"
#include <stdio.h>
#include <stdlib.h>

static int bit_reverse_by_idx(int idx, int n) //Разворот битов индекса
{
    int rev = 0; //Перевёрнутое значение

    for(int i = 0; i < n; i++) //По n
    {
        rev <<= 1; //Сдвиг
        rev |= (idx & 1); //Получение бита
        idx >>= 1; //Сдвиг
    }
    
    return rev; //Возврат
}

static void reverse_amps_by_bit(State *state) //Поредвигуть амплитуды за счёт битов
{
    for(int i = 0; i < state->amps.n; i++) //На все амплитуды
    {
        int h = bit_reverse_by_idx(i,state->n); //Получить новый индекс

        if(h > i) //Если он больше
        {
            //Меняем местами
            complex double temp = state->amps.arr[i].amplitude;
            state->amps.arr[i].amplitude = state->amps.arr[h].amplitude;
            state->amps.arr[h].amplitude = temp;
        }
    }
}

static void butterfly(State *state, int left, int right, complex double w) //Бабочка для FFT
{
    complex double u = state->amps.arr[left].amplitude, v = state->amps.arr[right].amplitude * w; //Получение новых значений

    state->amps.arr[left].amplitude = u + v; //Вычисление амплитуд
    state->amps.arr[right].amplitude = u - v;
}

static void linecalc(State *state, int start, int len, complex double wlen) //Вычисление линейного ряда
{
    complex double w = 1; //Старт с 1

    for(int i = 0; i < len/2; i++) //На всю длину
    {
        butterfly(state,start+i,start+i+len/2,w); //Получение амплитуд

        w *= wlen; //Изменение фазы
    }
}

static void fft_step(State *state, int len, int sign) //Шаг FFT
{
    complex double wlen = 0; //Множитель фазы

    if(sign < 0) //Определение знака
    {
        wlen = cexp(-2*M_PI*I/len);
    } else if(sign > 0)
    {
        wlen = cexp(2*M_PI*I/len);
    }

    for(int i = 0; i < state->N/len; i++) //Вычисление по длинам
    {
        linecalc(state,i*len,len,wlen);// Ну и само вычисление
    }
}

static void fft(State *state, int sign) //Быстрое преобразование Фурье
{
    int len = 2; //Начало с 2х

    while(len <= state->N) //Оно будет рости
    {
        fft_step(state,len,sign); //Шагаем

        len *= 2; //Увеличение отрезка
    }
}

static void normalize(State *state) //Нормализация амплитуд
{
    complex double k = 1/sqrt(state->N); //Коэффицент

    for(int i = 0; i < state->amps.n; i++) //Для всех амплитуд
    {
        state->amps.arr[i].amplitude *= k; //Вычисление
    }
}

void qft(State *state) //Квантовое преобразование Фурье
{
    reverse_amps_by_bit(state); //Перемешать амплитуды

    fft(state,1); //Преобразовать

    normalize(state); //Нормализовать
}

void iqft(State *state) //Аналогично, но у фазы другой знак
{
    reverse_amps_by_bit(state);

    fft(state,-1);

    normalize(state);
}