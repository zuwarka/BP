#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#define N 512

//10.4 Скопировать текстовый файл, зеркально поменяв порядок строк

void nullcheck(FILE* f)
{
	if (!f)
	{
		printf("Error opening file!!!\n");
		exit(1);
	}
}

int main()
{
	FILE* F1, * F2;
	char** text = NULL; //рабочий массив указателей на строки
	char initfile[300] = "C:\\Users\\Oldenburg\\Desktop\\ГУАП\\2 курс\\ОП 3 семестр\\lab 10 ОП\\ReadMe.txt";
	char resfile[300] = "C:\\Users\\Oldenburg\\Desktop\\ГУАП\\2 курс\\ОП 3 семестр\\lab 10 ОП\\Result.txt";
	char str[N]; //очередная считываемая строка
	char* este = NULL; //указатель для очередной считываемой строки
	int i = 0; //индекс считываемой строки
	int count = 0; //подсчет количества строк

	printf("The program has started...\n");
	F1 = fopen(initfile, "rt");
	nullcheck(F1);

	while (!feof(F1)) //считаем количество строк в файле и заносим их в массив
	{
		este = fgets(str, N, F1);
		if (este == NULL)
			break;
		count++;
		text = (char**)realloc(text, (i + 1) * sizeof(char*));
		text[i] = (char*)malloc((strlen(str)+1) * sizeof(char));
		strcpy(text[i], str);
		i++;
	}

	fclose(F1);
	F2 = fopen(resfile, "wt");
	nullcheck(F2);

	for (i = count-1; i >= 0; i--)
	{
		fputs(text[i], F2);
	}

	fclose(F2);
	for (i = 0; i < count; i++)
		free(text[i]);
	free(text);
	printf("The program has finished succesfully!\n");
	return 0;
}
