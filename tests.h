#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <complex.h>
#include "functions.h"

#if defined(NDEBUG) //Проверка типа сборки
#define ASSERTION "disabled"
#else
#define ASSERTION "enabled"
#endif

clock_t start;
FILE *log_file;
double eps = 1e-9; // Возможная погрешность double

#define ASSERT(cond, msg)\
    if (!(cond)) {\
        fprintf(log_file, "%s FAILED (%0.2lfsec) in %s line %d - %s\n",\
            test_name, (double)(clock()-test_start)/CLOCKS_PER_SEC, __FILE__, __LINE__, (msg));\
        fflush(log_file);\
        return 1;\
    }

int grover_test(State* state); // Тест алгоритма Гровера

int qft_BStest(State* state, int ver); // Тест QFT на базисных состояниях (ver - вывод амплитуд)

int qft_rand_test(State* state, int ver); // Тест QFT и IQFT на случайном состоянии (ver - вывод амплитуд)