#pragma once

#include "types.h"
#include "state.h"
#include <stdlib.h>
#include <stdio.h>

int rand_return_x0(State *state/*, double *u_return*/); //Вернуть индекс числа которое было наиболее вероятным

void save_amps(State *state); //Сохранить амплитуды в файл

void open_amps_file(const char *filename); //Открыть файл, где их хранить нужно

void close_amps_file(); //Закрыть файл

void save_amps_count(State *state); //Сохранить амплитуды состояния

void save_states_count(int count); //Сохранить кол-во состояний

void set_pos_to_states_count();

long get_file_pos();

void set_pos_to(long pos);

double norm_square_amps(State* state); // Найти сумму квадратов модулей амплитуд