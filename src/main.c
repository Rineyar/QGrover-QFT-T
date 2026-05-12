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
char* func_name = NULL; // Имя функции для обработчика ошибок. 
clock_t start;

State *state = NULL;

int QgroverAlg(void)
{
    func_name = "Grover";

    puts("\n======= АЛГОРИТМ ГРОВЕРА =======\n");
    state = malloc(sizeof(State));

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
    printf("%d\n", verbose);
    if (verbose) print_state(state, "Начальное состояние:", stdout);

    set_uniform_superposition(state);
    if (verbose) print_state(state, "После гейта Адамара", stdout);

    save_amps_count(state);

    int r = grover_iters(state);

    save_states_count(r+1);

    HANDLE(grover_alg(state, x0),\
        "Количество кубит меньше или равно нулю: N <= 0",\
        "Вычисленное количество итераций меньше нуля: r < 0",\
        "Искомый индекс вне диапазона: 0 <= x < N");
    
    if (verbose) print_state(state, "После алгоритма Гровера", stdout);
    
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
    
    printf("\nВремя выполнения: %lf сек.\n",((double)(clock()-start))/CLOCKS_PER_SEC); //Вывод времени работы

    return 0; //Успешное завершение программы
}

int QFT(State *state, int is_reverse)
{
    func_name = "QFT";
    if (is_reverse) puts("\n======= Обратное Преобразование Фурье =======");
    if (!is_reverse) puts("\n======= Преобразование Фурье =======");

    open_amps_file(NULL);

    int steps = 0;

    if(state == NULL)
    {
        return -1;
    } else if(state != NULL)
    {
        save_amps_count(state);

        long pos = get_file_pos();

        save_states_count(0);

        save_amps(state);

        steps = is_reverse ? iqft(state) : qft(state);

        set_pos_to(pos);

        save_states_count(steps+2);
    }

    close_amps_file();

    printf("Кол-во шагов: %d\n",steps);

    return 0;
}


int main(int argc, const char **argv)
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    func_name = "main";
    state = calloc(1, sizeof(State));
    
    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        verbose = 1;
    }
    printf("verbose: %d\n", verbose);

    puts("\n======= КВАНТОВЫЕ АЛГОРИТМЫ =======");
    
    while(1) 
    {
        INTINPUT(1, 4, &command, \
            "\n> Выберите алгоритм:\n\n[1] - Алгоритм Гровера\n[2] - Преобразование Фурье\n[3] - Обратное преобразование Фурье\n[4] - Вывод графика\n[-1] - Выход\n\nВвод: ");
        switch (command)
        {
            case 1:
                HANDLE(QgroverAlg(),
                    "1",
                    "2",
                    "3");
                break;

            case 2:
                command = -1;
                INTINPUT(1, 3, &command, \
                    "\n> Состояние:\n\n[1] - Оставить текущее\n[2] - Ввести состояние вручную\n[3] - Задать рандомное состояние\nВвод: ");

                if(command == 1)
                {
                    if (state == NULL) {
                        printf("Состояние не определено. Попробуйте снова\n=====================\n");
                        break;
                    }
                }
                if(command == 2)
                    set_state_manually(state);
                if (command == 3)
                    HANDLE(set_random_state(state),
                        "состояние не определено",
                        "2",
                        "3");

                if (verbose) print_state(state, "До алгоритма Преобразование Фурье", stdout);
                HANDLE(QFT(state, 0),
                    "состояние не определено",
                    "None",
                    "None");
                if (verbose) print_state(state, "После алгоритма Преобразование Фурье", stdout);

                break;
            case 3:
                INTINPUT(1, 3, &command, \
                    "\n> Состояние:\n\n[1] - Оставить текущее\n[2] - Ввести состояние вручную\n[3] - Задать рандомное состояние\nВвод: ");

                if(command == 1)
                {
                    if (state == NULL) {
                        printf("Состояние не определено. Попробуйте снова\n=====================\n");
                        break;
                    }
                }
                if(command == 2)
                    set_state_manually(state);
                if (command == 3)
                    HANDLE(set_random_state(state),
                        "состояние не определено",
                        "2",
                        "3");

                if (verbose) print_state(state, "До алгоритма Обратного Преобразование Фурье", stdout);
                HANDLE(QFT(state, 1),
                    "Состояние не определено",
                    "None",
                    "None");
                if (verbose) print_state(state, "После алгоритма Обратного Преобразование Фурье", stdout);

                break;

            case 4:
                system("python ./visual/draw_amps.py");
                break;

            case -1:
                return 0;

            default:
                break;
        }
    }

    return 0;
}