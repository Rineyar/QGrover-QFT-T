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
int n, N;
State* state;

#define ASSERT(cond, msg)\
    if (!(cond)) {\
        fprintf(log_file, "%s FAILED (%0.2lfsec) in %s line %d - %s\n",\
            test_name, (double)(clock()-test_start)/CLOCKS_PER_SEC, __FILE__, __LINE__, (msg));\
        fflush(log_file);\
        return 1;\
    }

// Найти сумму квадратов модулей амплитуд
double norm_square_amps(State* state) 
{
    double norm_squared = 0;

    for (int i = 0; i < state->N; ++i)
    {
        double complex cur;
        read_amp_by_idx(state, i, &cur); 
        double re = creal(cur);
        double im = cimag(cur);
        norm_squared+= re*re + im*im;
    }
    return norm_squared;
}
  
// Вывод всех амплитуд
void print_state(State *state, const char* msg)
{ 
    fprintf(log_file, "%s\n", msg);               
    for (int i = 0; i < state->N; ++i) 
    {
        double complex amp;
        read_amp_by_idx(state, i, &amp);
        double real = creal(amp);
        double imag = cimag(amp);
        fprintf(log_file, "Amplitude %d: %g + %gi\n", i, real, imag);
    }
    fprintf(log_file, "\n");
}

// 
int grover_test(int n)
{
    clock_t test_start = start;
    const char* test_name = "GROVER ALGORITHM";
    
    state = malloc(sizeof(State));
    
    N = 2 << (n - 1);
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

// Тест QFT на базисных состояниях
// n - число кубитов
// ver - вывод амплитуд
int qft_BStest(int n, int ver) 
{
    clock_t test_start = start;
    const char* test_name = "QFT ON BASIS STATES";

    state = malloc(sizeof(State));
    N = 2 << (n - 1);
    init_state(state, n, N); //Инициализация памяти

    //fprintf(log_file, "IN: n = %d\n    N = %d\n", n, N);

    for (int i = 0; i < N; ++i) //Перебираем состояния
    {
        Amp_Vec_destroy(&state->amps);
        init_state(state, n, N); //Инициализация памяти
        
        state->amps.n = 0; //Чистим амплитуды
        for(int i = 0; i < state->N; i++) //Создание неразряженного состояния
        {
            Amp_Vec_push(&state->amps, (Amp){.idx=i, .amplitude=0}); //Добавление амплитуды
        }

        set_amp_by_idx(state, 1, i);
        if (ver==1)
        {
            fprintf(log_file, "\n\n---STATE %d---\n\n", i);
            print_state(state, "> Before QFT:");
        }

        qft(state);

        if (ver==1) 
        {
            print_state(state, "> After QFT:");
            fprintf(log_file,"\n");
        }

        // Сумма квадратов модулей амплитуд равна 1
        double sq = norm_square_amps(state);
        ASSERT(fabs(sq - 1) <= eps, "Сумма квадратов модулей амплитуд не равна 1");

        // Проверка амплитуд
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
        if (ver==1) fprintf(log_file, "STATE %d QFT TEST PASSED!\n", i);
        
        fflush(log_file);  // Вывод после каждого теста
    }
    fprintf(log_file, "%s PASSED IN %lfsec\n\n", test_name, ((double)(clock()-test_start))/CLOCKS_PER_SEC);
    return 0;
}

int main(void)
{
    log_file = fopen("log_file.txt","w");

    start = clock();
    for (int n = 1; n <= 20; ++n)
    {
        fprintf(log_file, "\n===== TEST %d =====\n", n);
        grover_test(n);
        qft_BStest(n, 0);
    }

    
}