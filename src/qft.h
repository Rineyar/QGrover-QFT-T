#pragma once

#include "types.h"

int qft(State *state, int save_amps_to_file); //Квантовое преобразование Фурье

int iqft(State *state, int save_amps_to_file); //Обртное квантовое преобразование Фурье