#include "types.h"
#include "functions.h"
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#if defined(NDEBUG) //Проверка типа сборки
#define ASSERTION "disabled"
#else
#define ASSERTION "enabled"
#endif

clock_t start;
FILE *log_file;

int main(void)
{
    log_file = fopen("log_file.txt","w");
    start = clock();

    State* state = malloc(sizeof(State));
    int n = 0; // Число кубитов (1 >= n >= 20) 

    printf("Введите необходимое число кубитов не более 20\nn = ");
    while(scanf("%d",&n) != 1 || n <= 0 || n > 20)
    {
        if (scanf("%d",&n) != 1) 
        {
            printf("Неверный ввод. Попробуйте ещё раз.\n");
        }
        else
        {
            printf("Число вне допустимого диапазона. Попробуйте ещё раз.\n");
        }
        printf("n = ");
    }   

    int N = 2 << n;

    init_state(state, n, N);

    int x = 0; //Искомый элемент (0 >= x >= N - 1)

    printf("Введите искомый элемент - число от 0 до %d\nx = ", N - 1);
    while(scanf("%d",&x) != 1 || x < 0 || x > N - 1)
    {
        if (scanf("%d",&x) != 1) 
        {
            printf("Неверный ввод. Попробуйте ещё раз.\n");
        }
        else
        {
            printf("Число вне допустимого диапазона. Попробуйте ещё раз.\n");
        }
        printf("x = ");
    }   

    set_uniform_superposition(state);

    clear_state(state); //Чистка состояний
    printf("Время выполнения: %lf\n",((double)(clock()-start))/CLOCKS_PER_SEC); //Вывод времени работы
    fclose(log_file);
    return 0; //Успешное завершение программы
}