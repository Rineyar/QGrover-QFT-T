#pragma once

#include "types.h"

void init_state(State *state, int n, int N); //Инициализация хранилища состояний

void set_start_state(State *state); //Получение 0о квантового состояния (|0⟩)

void clear_state(State *state); //Чистка структуры

int search_amp_by_idx(const State *state, int idx); //Поиск индекса в массиве по её индексу

int set_amp_by_idx(State *state, complex double amp, int idx); //Установка нового значения амплитуды

int read_amp_by_idx(const State *state, int idx, complex double *return_amp); //Получение числового значения амплитуды

int remove_amp_by_i(State *state, int i); //Удаление амплитуды по индексу в массиве

int is_amp_null(const complex double amp); //Является ли амплитуда нулевой

// int set_random_state(State* state);