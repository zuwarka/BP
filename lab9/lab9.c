#include <stdio.h>
#define N 1024
#define NO 0
#define YES 1
#define _CRT_SECURE_NO_WARNINGS
//9.4 Скопировать содержимое текстового файла, удалив в каждой строке самое длинное слово

void nullcheck(FILE* f) //проверка на нуль
{
	if (!f)
	{
		printf("Error opening file!!!\n");
		exit(1);
	}
}

int main()
{
	char initfile[500] = "C:\\Users\\Oldenburg\\Desktop\\ГУАП\\2 курс\\ОП 3 семестр\\Project1\\F1.txt";
	char resultfile[500] = "C:\\Users\\Oldenburg\\Desktop\\ГУАП\\2 курс\\ОП 3 семестр\\Project1\\F2.txt";
	char str[N], res[N]; //входная и результирующая строка
	char* inptr = str, * outptr = res; //указатели для входной и результирующей строк
	char *wrdptr = str, *symptr = res; //указатели на начало слова и символ результирующей строки
	int lttr = 0, maxlttr = 0, foundwrd = NO; //кол-во букв, максимальное кол-во букв слова в строке, признак слова
	FILE* F1, * F2;

	printf("The program has started...\n");
	F1 = fopen(initfile, "rt");
	nullcheck(F1);
	F2 = fopen(resultfile, "wt");
	nullcheck(F2);

	while (!feof(F1))
	{
		maxlttr = 0;
		inptr = fgets(str, N, F1);
		if (inptr == NULL)
			break;

		do
		{
			if ((*inptr == ' ') || (*inptr == '\0') || (*inptr == '.') || (*inptr == ',')
				|| (*inptr == ':') || (*inptr == ';') || (*inptr == '\n'))
			{	//встретился разделитель
				if (lttr > maxlttr)
					maxlttr = lttr;
				lttr = 0; //обнуляем кол-во букв для следующего слова
			}
			else
				lttr++; //перед нами слово, считаем буквы
		} while (*inptr++ != '\0'); //находим самое длинное слово в строке, пока не встретим в ней нуль-терминатор

		inptr = str; //продолжаем работать со строкой

		do
		{
			if ((*inptr == ' ') || (*inptr == '\0') || (*inptr == '.') || (*inptr == ',')
				|| (*inptr == ':') || (*inptr == ';') || (*inptr == '\n'))
			{
				if ((foundwrd == YES) && (lttr != maxlttr)) //перед нами слово, не самое длинное
				{
					while (wrdptr < inptr) //пока адрес на начало слова меньше оного на строку,
						*symptr++ = *wrdptr++; //копируем в результирующую строку слово через указатель
				}

				if (*inptr != '\0') //копируем разделители
					*symptr++ = *inptr;
				foundwrd = NO;
				lttr = 0;
			}
			else
			{
				lttr++;
				if (foundwrd == NO)
					wrdptr = inptr; //запоминаем адрес начала слова
				foundwrd = YES; //найдено слово
			}
		} while (*inptr++ != '\0');
	}

	while (outptr != symptr) //посимвольно заносим результат в выходной файл до завершения, 
		fprintf(F2, "%c", *outptr++); //то есть, пока указатели на результируюущю строку не примут одинк=аковое значение

	fclose(F1);
	fclose(F2);
	printf("The program has finished succesfully!\n");

	return 0;
}
