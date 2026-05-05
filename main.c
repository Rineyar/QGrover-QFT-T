#include <complex.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "functions.h"

int verbose = 0; // Выводить состояния амплитуд? (не работает)
int command = 0; // Вызываемая функция
char* func_name = NULL; // Имя функции для обработчика ошибок. Если будет время, можно реализовать
                 // стеком для возможности полностью проследить, откуда ошибка пошла
clock_t start;

// Обработчик ошибок (возможно стоит объявить в другом месте. Нуждается в тестах)
#define HANDLE(err, c1, c2, c3)\
    switch (err)\
    {\
    case -1:\
        printf("OШИБКА в %s %s строка %d (%0.2lfsec) - %s\n",\
            __FILE__, func_name, __LINE__, (double)(clock()-start)/CLOCKS_PER_SEC, (c1));\
            return -1;\
    case -2:\
        printf("OШИБКА в %s %s строка %d (%0.2lfsec) - %s\n",\
            __FILE__, func_name, __LINE__, (double)(clock()-start)/CLOCKS_PER_SEC, (c2));\
            return -2;\
    case -3:\
        printf("OШИБКА в %s %s строка %d (%0.2lfsec) - %s\n",\
            __FILE__, func_name, __LINE__, (double)(clock()-start)/CLOCKS_PER_SEC, (c3));\
            return -3;\
    default:\
        break;;\
    }

// Пользовательский ввод. Число в диапазоне [min, max] записываем в addr. -1 -- экстренный выход
#define INTINPUT(min, max, addr, msg, ...)\
    printf((msg), ##__VA_ARGS__);\
    while(1)\
    {\
        if (scanf("%d", (addr)) != 1) {\
            while (getchar() != '\n'){\
                printf("Неверный ввод. Попробуйте ещё раз\nВвод: ");\
            }\
            continue;\
        }\
        if (*(addr) < (min) || *(addr) > (max)) {\
            if (*(addr)==-1){\
            return 0;}\
            else{\
            printf("Число вне допустимого диапазона. Попробуйте ещё раз\nВвод: ");}\
            continue;\
        }\
        break;\
    }   
    

void print_state(State *state, const char* msg) {
    printf("%s\n", msg);
    for (int i = 0; i < state->N; ++i) 
    {
        double complex amp;
        read_amp_by_idx(state, i, &amp);
        double real = creal(amp);
        double imag = cimag(amp);
        printf("Амплитуда %d: %g + %gi\n", i, real, imag);
    }
    printf("\n");
}

int QgroverAlg(void)
{
    puts("\n======= АЛГОРИТМ ГРОВЕРА =======\n");
    State* state = malloc(sizeof(State));

    int n = 0; // Число кубитов (1 >= n >= 20) 
    INTINPUT(1, 20, &n, "Введите необходимое число кубитов (1-20)\nВвод: ");

    int N = 2 << (n - 1); // Количество элементов

    int x0 = 0; //Искомый элемент (0 >= x >= N - 1)
    INTINPUT(1, N, &x0, "\nВведите искомый элемент (1-%d)\nВвод: ", N);
    x0--; // Корректируем смещение ввода пользователя
    putchar('\n');

    start = clock();

    open_amps_file(NULL);

    // Алгоритм Гровера
    init_state(state, n, N);
    if (verbose) print_state(state, "Начальное состояние:");

    set_uniform_superposition(state);
    if (verbose) print_state(state, "После гейта Адамара");

    save_amps_count(state);

    int r = grover_iters(state);

    save_states_count(r+1);

    HANDLE(grover_alg(state, x0),\
        "Количество кубит меньше или равно нулю: N <= 0",\
        "Вычисленное количество итераций меньше нуля: r < 0",\
        "Искомый индекс вне диапазона: 0 <= x < N");
    
    if (verbose) print_state(state, "После алгоритма Гровера");
    
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

    close_amps_file();

    clear_state(state); //Чистка состояний
    printf("\nВремя выполнения: %lf сек.\n",((double)(clock()-start))/CLOCKS_PER_SEC); //Вывод времени работы

    return 0; //Успешное завершение программы
}

//int QFT(state)


int main(int argc, const char **argv)
{
    
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        verbose = 1;
    }
    printf("verbose: %d\n", verbose); // Не читается

    puts("\n======= КВАНТОВЫЕ АЛГОРИТМЫ =======");
    INTINPUT(1, 4, &command, \
    "\n> Выберите алгоритм:\n\n[1] - Алгоритм Гровера\n[2] - Преобразование Фурье\n[3] - Обратное преобразование Фурье\n[-1] - Выход\n\nВвод: ");
    
    while(1)
    {
        switch (command)
        {
        case 1:
            QgroverAlg();
            INTINPUT(1, 4, &command, \
        "\n> Выберите алгоритм:\n\n[1] - Алгоритм Гровера\n[2] - Преобразование Фурье\n[3] - Обратное преобразование Фурье\n[-1] - Выход\n\nВвод: ");
            break;
        case -1:
            return 0;
        default:
            break;
        }
    }
    return 0;
}