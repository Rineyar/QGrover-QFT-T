#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#include "functions.h"

clock_t start;

int main(void)
{

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
    int x = 0; //Искомый элемент (0 >= x >= N - 1)

    e = 1;
    while(e)
    {
        if (scanf("%d",&x) != 1) {
            printf("Неверный ввод. Попробуйте ещё раз.\n");
            continue;
        }
        if (x < 1 || x > N) {
            printf("Число вне допустимого диапазона. Попробуйте ещё раз.\n");
            continue;
        }
        e = 0; // Ошибок не было
        x--; // Корректируем смещение ввода пользователя
    }   

    start = clock();

    init_state(state, n, N);
    set_uniform_superposition(state);
    int r = grover_iters(state);
    printf("%d", r);
    grover_alg(state, x);
    clear_state(state); //Чистка состояний

    printf("Время выполнения: %lf\n",((double)(clock()-start))/CLOCKS_PER_SEC); //Вывод времени работы
    return 0; //Успешное завершение программы
}