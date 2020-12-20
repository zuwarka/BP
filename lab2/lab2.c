#define _CRT_SECURE_NO_WARNINGS //чтобы использовать функцию scanf
#include <stdio.h>

int main(void)
{
	float s = 0; //сумма чисел, а также обнуление переменной
	int n, i; //переменная количества суммируемых элементов ряда и элемента ряда соответственно 
	printf("Input number of summing elements n = ");
	scanf("%d", &n);

	for (i = 1; i <= n; i++) //от первого элемента по n-ый с увеличением i на единицу на каждом шаге цикла
		s = s + i / (i * i + i * 2.0 + 1.0); //увеличение суммы по рекурентной формуле i-того элемента
	printf("The sum of n elements s = %.3f\n", s);
	return 0;
}
