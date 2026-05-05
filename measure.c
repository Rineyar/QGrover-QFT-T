#include "measure.h"
#include <stdlib.h>
#include <stdio.h>

int rand_return_x0(State *state/*, double *u_return*/) //Возвращает индекс наиболее вероятного
{
    double u = ((double)rand())/((double)RAND_MAX+1); //Шанс фозврата именно сейчас

    // *u_return = u; //В прошлом отладочная, можно убрать

    double sum = 0; //Сумма амплитуд

    for(int i = 0; i < state->amps.n; i++) //Гоняем по всем ненулевым
    {
        double campmod = cabs(state->amps.arr[i].amplitude); //Модуль комплексной амплитуды
        sum += campmod*campmod; //Добавить её вероятность в сумма

        if(sum >= u) //Если нашлось (или ложное срабатывание)
        {
            return state->amps.arr[i].idx; //Вернуть индекс
        }
    }

    return -1; //Что-то пошло не так
}

static FILE *saved_amps = NULL;

void open_amps_file(const char *filename)
{
    if(filename == NULL)
    {
        saved_amps = fopen("saved_amps.bin","wb");
    } else {
        saved_amps = fopen(filename,"wb");
    }
}

void close_amps_file()
{
    fclose(saved_amps);
}

void save_amps_count(State *state)
{
    fwrite(&state->amps.n,sizeof(int),1,saved_amps);
}

void save_amps(State *state)
{
    for(int i = 0; i < state->amps.n; i++)
    {
        fwrite(&state->amps.arr[i].amplitude,sizeof(complex double),1,saved_amps);
    }
}

void save_states_count(int count)
{
    fwrite(&count,sizeof(int),1,saved_amps);
}

void set_pos_to_states_count()
{
    fseek(saved_amps,16,SEEK_SET);
}

long get_file_pos()
{
    return ftell(saved_amps);
}

void set_pos_to(long pos)
{
    fseek(saved_amps,pos,SEEK_SET);
}