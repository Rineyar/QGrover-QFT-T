#include <stdio.h>
//#include <math.h>
//#include <complex.h>
#include <time.h>
#include "state.h"  

/*
__int128_t fastpow_mod(__int128_t a, __int128_t e, __int128_t mod) //быстрое возведение в степень на принципе бинарого сдвига
{
    __int128_t r = 1; //результат
    
    a %= mod; //возводимое

    while(e > 0)
    {
        if(e & 1) //проверка чётности
        {
            r = (r*a)%mod; //работа с нечётной степенью
        }

        a = (a*a)%mod; //работа с чётной степенью

        e >>= 1; //деление на 2
    }

    return r; //вывод
}
*/

int main(void) //Мейн
{
    clock_t runtimer = clock(); //Время старта
    FILE *errorlog = fopen("errorlog.txt","w"); //Файл с логами

    State state = {0}; //Память состояний

    short n = 0; //Кол-во кубитов 1-20
    if(scanf("%hi",&n) != 1) //Считать
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

    long int N = 1L << n; //Вычисление N

    init_state(&state,n,N); //Передача n/N в состояния

    if(state.n != n || state.N != N) //Проверка передечи n/N в состояния
    {
        fprintf(errorlog,"Ошибка передачи n/N в состояния\n"); //Вывод ошибки
        fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы
        fclose(errorlog);
        return 1; //Аварийное завершение
    }

    long int x0 = 0; //Искомый элемент
    if(scanf("%ld",&x0) != 1) //Считать
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

    fprintf(errorlog,"Время выполнения: %lf\n",((double)(clock()-runtimer))/CLOCKS_PER_SEC); //Вывод времени работы

    fclose(errorlog);
    return 0; //Успешное завершение программы
}