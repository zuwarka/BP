#include <stdio.h>
#include <stdlib.h> //для использования функции модуля числа
#define N 5 //задаем размерность массива
#define K 5
#define YES 1 //переменные признака геометрической прогрессии
#define NO 0
main()
{
	float q = 0; //знаменатель прогрессии
	int a[N][K], i = 0, j = 0, flag = NO; //flag - "флажок" прогрессии
	for (i = 0; i < N; i++) //вводим элементы массива с клавиатуры
		for (j = 0; j < K; j++)
			scanf_s("%d", &a[i][j]);
	for (i = 0; i < N; i++) //начинаем рассматривать элементы, построчно
	{
		if (abs(a[i][1]) > abs(a[i][0])) //если второй элемент i-ой строки больше первого,
		{
			q = a[i][1] / a[i][0]; //то знаменатель будет больше единицы
			for (j = 0; j < K - 1; j++) //перебираем К-1 элементов в массиве,
			{  //так как первые два уже рассмотрены в переменной q 
				if (a[i][j + 1] == q * a[i][j]) //следующий элемент есть произведение q на предыдущий
					flag = YES; //перед нами прогрессия
				else							
				{                               
					flag = NO; //элемент не вписывается в рамки прогрессии,
					break; //поэтому необходимо перейти на следующую строку
				}
			}
			if (flag == YES) //пропускает этот шаг, пока не закончится
			{		   //перебор всех элементов строки
				if (q == 1) //номер строки - это ее адрес+1
					printf("\nString number %d is a constant geometric progression", i + 1);
				else 
				{
					if (q < 0)
						printf("\nString number %d is an alternating geometric progression", i + 1);
					else
						printf("\nString number %d is an increasing geometric progression", i + 1);
				}
			}
		}
		else							
		{      //если первый элемент i-ой строки больше второго
			q = a[i][0] / a[i][1]; //знаменатель меньше единицы (вида 1/q)
			for (j = 0; j < K - 1; j++)
			{
				if (a[i][j] == q * a[i][j+1]) //следующий элемент есть произведение q на предыдущий
					flag = YES;
				else
				{
					flag = NO;
					break;
				}
			}
			if (flag == YES)
			{
				if (q == 1)
					printf("\nString number %d is a constant geometric progression", i + 1);
				else
				{
					if (q < 0)
						printf("\nString number %d is an alternating geometric progression", i + 1);
					else
						printf("\nString number %d is a decreasing geometric progression", i + 1);
				}
			}
		}
	}
	return 0;
}
