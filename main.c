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

    set_start_state(&state); //Получение стартового состояния

    set_uniform_superposition(&state); //Получение универсальной суперпозиции

    complex double out = 0;

    for(int i = 0; i < state.N; i++)
    {
        read_amp_by_idx(&state,i,&out);

        printf("%d: %lf +i%lf\n",i,creal(out),cimag(out));
    }

    read_amp_by_idx(&state,x0,&out);

    printf("%lf +i%lf\n",creal(out),cimag(out));

    grover_alg(&state,x0);

    read_amp_by_idx(&state,x0,&out);

    printf("%lf +i%lf\n",creal(out),cimag(out));

    for(int i = 0; i < state.N; i++)
    {
        read_amp_by_idx(&state,i,&out);

        printf("%d: %lf +i%lf\n",i,creal(out),cimag(out));
    }

    read_amp_by_idx(&state,x0,&out);

    printf("P(x) - %lf\n",cabs(out)*cabs(out));

    // read_amp_by_idx(&state,4,&out);

    // printf("%lf +i%lf\n",creal(out),cimag(out));

    // printf("%lf +i%lf\n",creal(state.amps.arr[x0].amplitude),cimag(state.amps.arr[x0].amplitude));

    // oracle(&state,x0);

    // diffusion(&state);

    // read_amp_by_idx(&state,4,&out);

    // printf("%lf +i%lf\n",creal(out),cimag(out));

    // printf("%lf +i%lf\n",creal(state.amps.arr[x0].amplitude),cimag(state.amps.arr[x0].amplitude));

    clear_state(&state); //Чистка состояний
    fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
    fclose(errorlog);
    return 0; //Успешное завершение программы
}