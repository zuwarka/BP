/*8.4 В символьной строке удалить все слова,
состоящие более чем из N букв*/
#include <stdio.h>
#define N 1024
#define NO 0
#define YES 1

void del_lttr(char buff[], int num); //функция удаления слов
int main()
{
	char str[N];
	int num = 0, i = 0;
	printf("Input your string here: ");
	gets(str);
	printf("Input a value, by which one you about to delete words: ");
	scanf_s("%d", &num);
	del_lttr(str, num);
	printf("Your string is: ");
	while (str[i] != '\0') //цикл для вывода слов в нормальном виде
	{
		if (((str[i] == ' ') && (i == 0)) || (str[i] == '.'))
			//если первый символ - это пробел или точка, то переходим дальше
			i++;
		else
			if (((str[i - 1] == ' ') && (str[i] == ' ')) || ((str[i - 1] == '.') && (str[i] == '.')))
				//если предыдущий и нынешний символы - пробелы, точки
				i++;
			else
			{
				//окончательный вывод результирующего массива
				printf("%c", str[i]);
				i++;
			}
	}
	return 0;
}

void del_lttr(char buff[], int n)
{
	char c;
	int mark = NO, found = NO, lttr = 0; //марки на слово и искомый признак; количество букв в слове 
	char* inwrd; //указатель на настоящий символ данной строки
	char* outwrd; //указатель на настоящий символ результирующей строки
	char* wrdwrd; //указатель на начало слова

	inwrd = buff;
	outwrd = buff;
	wrdwrd = buff;

	do {
		c = *inwrd; //берем текущий символ из буферной строки, на которую указывал inwrd
		if (c == ' ' || c == '.' || c == ',' || c == '\n' || c == '\0')
		{
			if (mark == YES) //начато новое слово
			{
				if (found == NO) //искомый признак количества букв
				{
					while (wrdwrd < inwrd)     //записываем слово по признаку в результируюущую строку, пока
						*outwrd++ = *wrdwrd++; //указатель начала слова меньше указателя текущего символа
				}
			}
			mark = NO; //слово закончилось или не начиналось
			*outwrd++ = c; //переходим на следующий адрес символов
			lttr = 0;
		}
		else //началось слово
		{
			lttr++;
			if (mark == NO)
				wrdwrd = inwrd; //запоминаем адрес ячейки, где начинается слово
			if (lttr > n) //проверка на соответствие заданию
				found = YES;
			else
				found = NO;
			mark = YES; //признак нахождения в слове
		}
		inwrd++; //переходим к следующему символу
	} while (c != '\0');
}
