#include <stdio.h>
//#include <math.h>
#include <complex.h>
#include <time.h>
#include "state.h"

int main(void) //Мейн
{
    clock_t runtimer = clock(); //Время старта
    FILE *errorlog = fopen("errorlog.txt","w"); //Файл с логами

    State state = {0}; //Память состояний

    int n = 0; //Кол-во кубитов 1-20
    if(scanf("%d",&n) != 1) //Считать
    {
        fprintf(errorlog,"Ошибка чтения n!\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }   

    if(n <= 0 || n > 20) //Проверка ввода
    {
        fprintf(errorlog,"Некорректное кол-во кубитов!\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }

    int N = 1L << n; //Вычисление N

    init_state(&state,n,N); //Передача n/N в состояния

    if(state.n != n || state.N != N) //Проверка передечи n/N в состояния
    {
        fprintf(errorlog,"Ошибка передачи n/N в состояния\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }

    int x0 = 0; //Искомый элемент
    if(scanf("%d",&x0) != 1) //Считать
    {
        fprintf(errorlog,"Ошибка чтения x0\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }

    if(x0 < 0 || x0 >= N) //Проверка ввода
    {
        fprintf(errorlog,"Некорректный искомый элемент\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }

    get_start_amp(&state);

    printf("n - %d\nN - %d\ncount - %d\namp_idx - %d\namp_r - %lf\namp_i - %lf\n",state.n,state.N,state.amps.n,state.amps.arr[0].idx,creal(state.amps.arr[0].amplitude),cimag(state.amps.arr[0].amplitude));

    fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
    fclose(errorlog);
    return 0; //Успешное завершение программы
}