#include <complex.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "gates.h"
#include "state.h"
#include "grover.h"
#include "measure.h"

clock_t start;

void print_state(State *state, const char* msg) {
    printf("%s\n", msg);
    for (int i = 0; i < state->N; ++i) {
        complex double amp = state->amps.arr[search_amp_by_idx(state, i)].amplitude;
        double real = creal(amp);
        double imag = cimag(amp);
        printf("Amplitude %d: %g", i + 1, real);
        if (imag) printf(" + %gi", imag);
        putchar('\n');
    }
    putchar('\n');
}

int main(int argc, const char **argv)
{
    int verbose = 0;
    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        verbose = 1;
    }

    State* state = malloc(sizeof(State));
    int n = 0; // Число кубитов (1 >= n >= 20) 

    puts("Введите необходимое число кубитов (1-20):");
    int e = 1;
    while(e)
    {
        if (scanf("%d",&n) != 1) {
            puts("Неверный ввод. Попробуйте ещё раз:");
            continue;
        }
        if (n < 1 || n > 20) {
            puts("Число вне допустимого диапазона. Попробуйте ещё раз:");
            continue;
        }
        e = 0;// Ошибок не было
    }   
    int N = 2 << (n - 1); // Количество элементов

    printf("Введите искомый элемент (1-%d):\n", N);
    int x0 = 0; //Искомый элемент (0 >= x >= N - 1)

    e = 1;
    while(e)
    {
        if (scanf("%d",&x0) != 1) {
            printf("Неверный ввод. Попробуйте ещё раз.\n");
            continue;
        }
        if (x0 < 1 || x0 > N) {
            printf("Число вне допустимого диапазона. Попробуйте ещё раз.\n");
            continue;
        }
        e = 0; // Ошибок не было
        x0--; // Корректируем смещение ввода пользователя
    }   

    start = clock();

    // Алгоритм Гровера
    init_state(state, n, N);
    if (verbose) print_state(state, "Начальное состояние:");

    set_uniform_superposition(state);
    if (verbose) print_state(state, "После что-то там");

    grover_alg(state, x0);
    if (verbose) print_state(state, "После алгоритма Гровера");

    int r = grover_iters(state);
    printf("Количество итераций: %d\n", r);

    // Поиск правильного x
    int measured_x = rand_return_x0(state);
    printf("Измерение: x = %d\n", measured_x + 1);

    // Измерение шанса выдачи правильного ответа
    int hits = 0;
    int trials = 10000;
    for (int t = 0; t < trials; t++)
        if (rand_return_x0(state) == x0) hits++;
    printf("P(%d) ≈ %f\n", x0 + 1, (double)hits / trials);

    // Количество ненулевых состояний
    printf("Ненулевых состояний: %d\n", state->amps.n);

    clear_state(state); //Чистка состояний
    printf("\nВремя выполнения: %lf сек.\n",((double)(clock()-start))/CLOCKS_PER_SEC); //Вывод времени работы

    return 0; //Успешное завершение программы
}