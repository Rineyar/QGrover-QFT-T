// Обработчик ошибок
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