#include <complex.h>

typedef struct amp //Структура для хранения амплитуд
{
    long int idx; //индекс
    complex double amplitude; //амплитуда
} Amp;

//Создание вектора Amp_Vec с типом Amp
#define TYPE Amp
#define NAME Amp_Vec
#include "vector/array_decl.h"

typedef struct state //структура хранения состояний
{
    short n; //Кубиты
    long int N; //Кол-во элементов
    Amp_Vec amps; //амплитуды
} State;

void init_state(State *state, short n, long int N); //Инициализация хранилища состояний

void get_start_amp(State *state); //Создание первой амплитуды