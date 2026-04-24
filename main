#include "types.h"
#include "functions.h"
#include <stdio.h>
#include <assert.h>
#include <time.h>

#if defined(NDEBUG) //Проверка типа сборки
#define ASSERTION "disabled"
#else
#define ASSERTION "enabled"
#endif

FILE *errorlog = NULL; //Файл с логами
clock_t runtimer = 0; //Время старта

/*
ToDo:
1) Сделать общий обработчик ошибок
2) Прописать дебагеры по флагу сборки
3) Расширить log систему
4) Дальше по фактам, добавлять че надо
*/

int main(void) //Мейн
{
    errorlog = fopen("errorlog.txt","w");

    runtimer = clock();

    fprintf(errorlog,"Asserts - %s\n",ASSERTION); //Дебаг или нет

    State state = {0}; //Память состояний

    int n = 0; //Кол-во кубитов 1-20
    if(scanf("%d",&n) != 1) //Считать
    {
        clear_state(&state); //Чистка состояний
        fprintf(errorlog,"Ошибка чтения n!\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }   

    if(n <= 0 || n > 20) //Проверка ввода
    {
        clear_state(&state); //Чистка состояний
        fprintf(errorlog,"Некорректное кол-во кубитов!\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }

    int N = 1L << n; //Вычисление N

    init_state(&state,n,N); //Передача n/N в состояния

    fprintf(errorlog,"Init-State: n - %d N - %d sn - %d sN - %d | ",n,N,state.n,state.N); //Залить инит в логи
    fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы

    if(state.n != n || state.N != N) //Проверка передечи n/N в состояния
    {
        clear_state(&state); //Чистка состояний
        fprintf(errorlog,"Ошибка передачи n/N в состояния\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }

    int x0 = 0; //Искомый элемент
    if(scanf("%d",&x0) != 1) //Считать
    {
        clear_state(&state); //Чистка состояний
        fprintf(errorlog,"Ошибка чтения x0\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }

    if(x0 < 0 || x0 >= N) //Проверка ввода
    {
        clear_state(&state); //Чистка состояний
        fprintf(errorlog,"Некорректный искомый элемент\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }

    set_uniform_superposition(&state);

    for(int i = 0; i < state.amps.n; i++)
    {
        printf("%d %lf + i%lf\n",i,creal(state.amps.arr[i].amplitude),cimag(state.amps.arr[i].amplitude));
    }

    printf("POST QFT:\n");

    qft(&state);

    for(int i = 0; i < state.amps.n; i++)
    {
        printf("%d %lf + i%lf\n",i,creal(state.amps.arr[i].amplitude),cimag(state.amps.arr[i].amplitude));
    }

    clear_state(&state); //Чистка состояний
    fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
    fclose(errorlog);
    return 0; //Успешное завершение программы
}