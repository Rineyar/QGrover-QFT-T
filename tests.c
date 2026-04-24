#include "functions.h"
#include "time.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <complex.h>
#include <types.h>

#if defined(NDEBUG) //Проверка типа сборки
#define ASSERTION "disabled"
#else
#define ASSERTION "enabled"
#endif

clock_t start;
FILE *log_file;

#define ASSERT(cond, msg)\
if (!(cond)) {\
    fprintf(log_file, "TEST FAILED: %s:%d - %s\n", __FILE__, __LINE__, (msg));\
    exit(EXIT_FAILURE);\
}

int grover_test()
{
    State* state = malloc(sizeof(State));
    int n = 4;
    int N = 2 << n;
    
    init_state(state, n, N); //Инициализация памяти
    set_uniform_superposition(state); //Установка стартовой амплитуды 1/sqrt(N)
    for (int i = 0; i < N; ++i)
    {
        double real = creal(state->amps.arr[search_amp_by_idx(state, i)].amplitude);
        fprintf(log_file, "%lf ", real);
    }
    putc('\n', log_file);

    int x = 0;
    grover_alg(state, x);
    
    for (int i = 0; i < N; ++i)
    {
        double real = creal(state->amps.arr[search_amp_by_idx(state, i)].amplitude);
        fprintf(log_file, "%lf ", real);
    }
    putc('\n', log_file);

    complex double x_amp;
    read_amp_by_idx(state, x, &x_amp);
    ASSERT(cabs(x_amp)*cabs(x_amp)>0.9, "wrong");

    return 0;
}

int main(void)
{
    log_file = fopen("log_file.txt","w");

    start = clock();
    grover_test();

    //fprintf(log_file,"Asserts - %s\n",ASSERTION); //Дебаг или нет

    

    /*int n = 0; //Кол-во кубитов 1-20
    if(scanf("%d",&n) != 1) //Считать
    {
        fprintf(log_file,"Ошибка чтения n!\n"); //Вывод ошибки
        fprintf(log_file,"Время выполнения: %lf\n",((double)(clock()-start))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(log_file);
        return 1; //Аварийное завершение
    }  */ 
}