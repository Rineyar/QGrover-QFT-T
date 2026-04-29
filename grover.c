#include "grover.h"

int grover_step(State *state, int x0) //Шаг алгоритма гровера
{
    //func_name = "grover_step";

    int cor = oracle(state,x0); //Вызор окарула

    if(cor > 0) //если сработал
    {
        diffusion(state); //Диффузия
        return 1;
    } else {
        return cor;
    }   
}

int grover_iters(State *state) //Кол-во итераций
{
    //func_name = "grover_iters";

    int r = (int)(M_PI_4 * sqrt(state->N)); //Вычисление

    return r; //Возврат
}

int grover_alg(State *state, int x0) //Сам алгоритм
{
    //func_name = "grover_alg";
    
    if (state->N <= 0) return -1;
    int r = grover_iters(state); //Кол-во шагов
    if (r < 0) return -2;
    
    int cor = 0; //Проверка

    for(int i = 0; i < r; i++) //Шагаем
    {
        cor = grover_step(state,x0); //Шаг
        
        if(cor < 0) //Если ошибка во время выполнения
        {
            return -3;
        }
    }

    return 0; //Возврат
}