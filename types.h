#pragma once

#include <complex.h>
#define _USE_MATH_DEFINES
#include <math.h>

typedef struct amp //Структура для хранения амплитуд
{
    int idx; //индекс
    complex double amplitude; //амплитуда
} Amp;

//Создание вектора Amp_Vec с типом Amp
#define TYPE Amp
#define NAME Amp_Vec
#include "vector/array_decl.h"

typedef struct state //структура хранения состояний
{
    int n; //Кубиты
    int N; //Кол-во элементов
    Amp_Vec amps; //амплитуды
} State;