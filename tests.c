#include "tests.h"
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

int grover_test(State* state)
{
    clock_t test_start = start;
    const char* test_name = "GROVER ALGORITHM";
    
    int n = state->n;
    int N = state->N;
    int x = rand() % N;

    fprintf(log_file, "IN: n = %d, N = %d, x = %d\n", n, N, x);

    // Очищаем вектор
    Amp_Vec_destroy(&state->amps);
    state->amps.n = 0;
    Amp_Vec_init(&state->amps);

    // Установка стартовой амплитуды 1/sqrt(N)
    set_uniform_superposition(state); 

    int e = grover_alg(state, x);
    ASSERT(e != -1, "Количество кубит меньше или равно нулю: N <= 0"); // Совершенно бесполезно, тк зависит от n, определяемого при тестировании
    ASSERT(e != -2, "Вычисленное количество итераций меньше нуля: r < 0"); 
    ASSERT(e != -3, "Искомый индекс вне диапазона: 0 <= x < N"); // Тоже самое
    
    // Проверка P(x) -> 1
    double complex Pc;
    read_amp_by_idx(state, x, &Pc);
    double P = cabs(Pc) * cabs(Pc);
    ASSERT(P > 0.9, "Вероятность слишком низкая: P(x) <= 0.9");

    fprintf(log_file, "GROVER OUT: x = %d, P(x) = %lf\n", x, P);
    fprintf(log_file, "%s PASSED IN %lfsec\n", test_name, ((double)(clock()-test_start))/CLOCKS_PER_SEC);

    // Вывод после каждого теста
    fflush(log_file);  

    return 0;
}

int qft_BStest(State* state, int ver) 
{
    clock_t test_start = start;
    const char* test_name = "QFT ON BASIS STATES";

    int n = state->n;
    int N = state->N;

    for (int i = 0; i < N; ++i) //Перебираем состояния
    {
        // Очистка амплитуд
        Amp_Vec_destroy(&state->amps);
        state->amps.n = 0;
        Amp_Vec_init(&state->amps);

        // Заполняем нулями
        for(int i = 0; i < state->N; i++)
        {
            Amp_Vec_push(&state->amps, (Amp){.idx=i, .amplitude=0});
        }
        // i-ая амплитуда - 1
        set_amp_by_idx(state, 1, i);

        if (ver==1)
        {
            fprintf(log_file, "\n----- STATE %d ----------------------------------------------------------------\n", i + 1);
            print_state(state, "> Before QFT:", log_file);
        }

        qft(state);

        if (ver==1) 
        {
            print_state(state, "> After QFT:", log_file);
        }

        //===== ПРОВЕРКИ =====

        // Сумма квадратов модулей амплитуд равна 1
        double sq = norm_square_amps(state);
        ASSERT(fabs(sq - 1) <= eps, "Сумма квадратов модулей амплитуд не равна 1");

        // Проверка модулей амплитуд (все равны 1/sqrt(N))
        double complex exp = 1/sqrt(state->N);

        for (int j = 0; j < N; ++j)
        {
            double complex cur;
            read_amp_by_idx(state, j, &cur);

            cur = cabs(cur);

            ASSERT(fabs(cur - exp) <= eps, "Амплитуды не соответствуют ожидаемым");
        }

        // Проверка фаз 
        for (int j = 0; j < N; ++j)
        {
            exp = (2.0 * M_PI * i * j) / N;
            // Выравнивание к диапазону [-π, π]
            exp = fmod(exp, 2.0 * M_PI);
            if (creal(exp) > M_PI) exp -= 2.0 * M_PI;
            if (creal(exp) <= -M_PI) exp += 2.0 * M_PI;

            double complex cur;
            read_amp_by_idx(state, j, &cur);
            double phase = carg(cur);

            double diff = fabs(phase - exp);
            // Учитываем скачок на границе π/-π
            if (diff > M_PI) diff = 2.0 * M_PI - diff;
            ASSERT(diff <= eps, "Фаза не соответствует ожидаемой");
        }
        if (ver==1) fprintf(log_file, "STATE %d QFT TEST PASSED!\n", i + 1);
        
        fflush(log_file);  // Вывод после каждого теста
    }
    
    fprintf(log_file, "%s PASSED IN %lfsec\n", test_name, ((double)(clock()-test_start))/CLOCKS_PER_SEC);
    return 0;
}

int qft_rand_test(State* state, int ver)
{
    clock_t test_start = start;
    const char* test_name = "QFT/IQFT ON RANDOM STATE";
    
    int n = state->n;
    int N = state->N;

    // Очищаем вектор
    Amp_Vec_destroy(&state->amps);
    state->amps.n = 0;
    Amp_Vec_init(&state->amps);

    // Задаём случайное состояние
    set_random_state(state);
    
    if (ver==1)
    {
        fprintf(log_file, "\n---RANDOM STATE---\n");
        print_state(state, "> Before QFT:", log_file);
    }

    // Запоминаем состояние амплитуд
    double complex* mem = malloc(sizeof(double complex) * N);
    for (int i = 0; i < N; ++i)
    {
        read_amp_by_idx(state, i, &mem[i]);
    }

    qft(state);

    if (ver==1) 
    {
        print_state(state, "> After QFT:", log_file);
    }

    // Проверяем норму (должна быть 1)
    double norm = norm_square_amps(state);
    double diff = fabs(norm - 1);
    ASSERT(diff <= eps, "Норма не сохраняется");

    iqft(state);

    // Проверяем iqft
    for (int i = 0; i < N; ++i)
    {
        double complex amp;
        read_amp_by_idx(state, i, &amp);
        double diff_re = fabs(creal(amp)-creal(mem[i]));
        double diff_im = fabs(cimag(amp)-cimag(mem[i]));
        ASSERT(diff_re <= eps && diff_im <= eps, "Ошибка с обратным Фурье");
    }
    free(mem);

    fprintf(log_file, "%s PASSED IN %lfsec\n", test_name, ((double)(clock()-test_start))/CLOCKS_PER_SEC);
    return 0;
}

int main(void)
{
    log_file = fopen("log_file.txt","w");
    start = clock();

    State* state = malloc(sizeof(State));
    for (int n = 1; n <= 5; ++n)
    {
        int N = 2 << (n - 1);
        init_state(state, n, N); //Инициализация памяти

        fprintf(log_file, "\n===== TEST %d ================================================================\n", n);

        grover_test(state);
        qft_BStest(state, 0);
        qft_rand_test(state, 0);
    }
}