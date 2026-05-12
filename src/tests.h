#pragma once
#include "types.h"

int grover_test(State* state); // Тест алгоритма Гровера

int qft_BStest(State* state, int ver); // Тест QFT на базисных состояниях (ver - вывод амплитуд)

int qft_rand_test(State* state, int ver); // Тест QFT и IQFT на случайном состоянии (ver - вывод амплитуд)