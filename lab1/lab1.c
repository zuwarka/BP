#include <stdio.h>
int main(void)
{
	float V, t, u1; //Данные объема бассейна, время опустошения, скорости вытекания воды соответственно

	printf("The capacity of the pool (l), V = ");
	scanf_s("%f", &V); //Была использована рекомендованная отладчиком функция     scanf_s 

	printf("Empty time (min), t = ");
	scanf_s("%f", &t);

	printf("The speed of leakage (l/min), u1 = ");
	scanf_s("%f", &u1);

	printf("The speed of filling the pool, u2 = %.2f\n", u1-V/t);
		//Формула нахождения скорости приведена в функции printf
		//Ответ будет дан с двумя знаками после запятой
	return 0;
}
