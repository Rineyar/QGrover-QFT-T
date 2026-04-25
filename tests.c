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

#define ASSERT(cond, msg)\
    if (!(cond)) {\
        fprintf(log_file, "%s FAILED (%0.2lfsec) in %s line %d - %s\n",\
            test_name, (double)(clock()-test_start)/CLOCKS_PER_SEC, __FILE__, __LINE__, (msg));\
        fflush(log_file);\
        return 1;\
    }

int grover_test(int n)
{
    clock_t test_start = start;
    const char* test_name = "GROVER ALGORITHM";
    
    State* state = malloc(sizeof(State));
    
    int N = 2 << n;
    int x = rand() % N;

    fprintf(log_file, "IN: n = %d, x = %d\n", n, x);

    init_state(state, n, N); //Инициализация памяти
    set_uniform_superposition(state); //Установка стартовой амплитуды 1/sqrt(N)

    int e = grover_alg(state, x);
    ASSERT(e != -1, "Количество кубит меньше или равно нулю: N <= 0"); // Совершенно бесполезно, тк зависит от n, определяемого при тестировании
    ASSERT(e != -2, "Вычисленное количество итераций меньше нуля: r < 0"); 
    ASSERT(e != -3, "Искомый индекс вне диапазона: 0 <= x < N"); // Тоже самое
    
    /*Проверка амплитуд (удалить или использовать в другом месте)
    fprintf(log_file, "AMPS: ");
    for (int i = 0; i < N; ++i)
    {
        double complex amp_c;
        read_amp_by_idx(state, i, &amp_c);
        double amp = cabs(amp_c);
        fprintf(log_file, "%lf ", amp);
    }
    fprintf(log_file, "\n");*/

    // Проверка P(x) -> 1
    double complex Pc;
    read_amp_by_idx(state, x, &Pc);
    double P = cabs(Pc) * cabs(Pc);
    ASSERT(P > 0.9, "Вероятность слишком низкая: P(x) <= 0.9");

    fprintf(log_file, "OUT: x = %d, P(x) = %lf\n", x, P);
    fprintf(log_file, "%s PASSED IN %lfsec\n", test_name, ((double)(clock()-test_start))/CLOCKS_PER_SEC);

    clear_state(state); // Очистка памяти
    fflush(log_file);  // Вывод после каждого теста

    return 0;
}

//int qft_test()

int main(void)
{
    log_file = fopen("log_file.txt","w");

    start = clock();
    for (int n = 1; n <= 20; ++n)
    {
        fprintf(log_file, "\n- TEST %d -\n", n);
        grover_test(n);
    }
}