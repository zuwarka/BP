#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#define NO 0
#define YES 1

unsigned long equbit(unsigned long N, int bit) //функция выделения бита из числа для
{                           //последующего сравнения на симметрию
	return ((N & (1 << bit)) >> bit);
}

int symmetry(unsigned long N, int power)  //функция проверки на симметричность числа
{
	int flag = NO;                        //признак симметрии
	for (int i = 0; i < power; i++)       
	{                                     
		if (equbit(N, i) == equbit(N, power - i - 1)) //т.к. разряды нумеруются с нуля,
		{                                             //то чтобы найти и сравнивать старший разряд,
			flag = YES;                            //мы из мощности вычитаем единицу,
			continue;                              //что будет равно числу старшего разряда
		}
		else //если симметрично расположенные биты не равны, 
		{    //то возвращаем признак несимметричности и выходим из цикла
			flag = NO;
			break;
		}
	}
	return flag;
}

int main(void)
{
	unsigned long N = 0, K=0;    //длинное (беззнаковое) целое число и его копия
	int power = 0; //мощность числа

	printf("Input your hexadecimal number N: ");
	scanf_s("%x", &N); //вводим шестнадцатиричное число
	K = N;

	for (power = 0; K > 0; power++) //ищем мощность числа, деля его копию поразрядно
		K /= 2;
	if (symmetry(N, power))
		printf("Your number is symmetric in a binary system!\n");
	else
		printf("Your number isn't symmetric in a binary system!\n");
	return 0;
}
