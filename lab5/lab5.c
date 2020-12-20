#include <stdio.h> 
#define N 10  
main()
{
    int i = 0, a[N], negnum = 0, posnum = 0; //сумма отрицательных и положительных чисел
    for (i = 0; i < N; i++) //задаем массив вводом с клавиаутры
        scanf_s("%d", &a[i]);
    for (i = 0; i < N; i++) 
    {
        if (a[i] < 0) //элемент массива отрицательный
            negnum += a[i]; //суммируем отрицательные числа
        else
            posnum += a[i]; //суммируем положительные числа
    }
    negnum = -negnum; //условно "модуль" отрицательных чисел
    if (negnum < posnum) //если модуль суммы отрицательных меньше суммы положительных
    {
        for (i = 0; i < N; i++)
        {
            if (a[i] < 0)
                a[i] = 0; //обнуляем отрицательные
        }
    }
    printf("Sum of positive numbers: %d\n", posnum);
    printf("Sum of negative numbers: %d\n", negnum);
    for(i = 0; i < N; i++) //выводим получившийся массив
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}
