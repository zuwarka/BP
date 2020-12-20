#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <locale.h>
#define YES 1
#define NO 0
#define N 100

int main()
{
		setlocale(LC_ALL, "Rus");
		char direct[12][12]; //массив директив

		strcpy(direct[0], "include");
		strcpy(direct[1], "define");
		strcpy(direct[2], "undef");
		strcpy(direct[3], "if");
		strcpy(direct[4], "ifdef");
		strcpy(direct[5], "ifndef");
		strcpy(direct[6], "else");
		strcpy(direct[7], "elif");
		strcpy(direct[8], "endif");
		strcpy(direct[9], "line");
		strcpy(direct[10], "error");
		strcpy(direct[11], "pragma");

		HANDLE hStdout;
		WORD foregroundColor0;
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		foregroundColor0 = FOREGROUND_GREEN;
		SetConsoleTextAttribute(hStdout, foregroundColor0);

		FILE* f = fopen("Readme.txt", "r");
		if (!f)
		{
			printf("Error opening!\n");
			exit(1);
		}

		char string[N]; //считываемая строка
		char* ptr = NULL; //указатель на нее
		char buf[12]; //массив слова (директива)
		char *nextptr; //указатель на следующий элемент строки

		while (!feof(f))
		{
			ptr = fgets(string, N, f);
			if (ptr == NULL)
				break;

			while ((*ptr != '\n') && (*ptr != '\0')) //пока не дойдем до терминаторов
			{
				int i = 0;
				if (*ptr == '#') //перед нами признак директивы
				{
					nextptr = ptr + 1; //перешагиваем на начало команды директивы

					do
					{	//заносим команду в буфер посимвольно, пока не встретим терминатор и пока буфер вмещает наши символы
						buf[i] = *nextptr; 
					} while ((*nextptr != ' ') && (i++ != 12) && (*nextptr != '\0') && (*nextptr++ != '\n'));

					i = 0;
					int j = 0;
					int found = NO;

					for (j = 0; j < 12; j++)
					{
						i = 0;
						do
						{	//чтобы сравнивались только буквы
							if (buf[i] != direct[j][i]) { found = YES; break; } //если вдруг перед нами не директива
							i++;
						} while ((buf[i] != ' ') && (direct[j][i] != '\0'));

						if ((found == NO) && ((buf[i] == ' ') || (buf[i] == '\0') || (buf[i] == '\n')) && (direct[j][i] == '\0'))
						{	//если конечный символ в буфере - терминатор и конечный символ в массиве директив - тоже, то мы нашли команду, ставим цвет
							SetConsoleTextAttribute(hStdout, foregroundColor0);
							break;
						}

						found = NO;
					}
				}

				while ((*ptr != ' ') && (*ptr != '\n') && (*ptr != '\0'))
				{	//печатаем слово в цвете до разделителей или терминаторов
					printf("%c", *ptr++);
				}

				if (*ptr != '\0')
				{	//если строка кончилась, то выводим терминатор
					printf("%c", *ptr++);
				}
				//далее все будет выводиться в белом цвете
				SetConsoleTextAttribute(hStdout, 7);
			}

			if (*ptr == '\n')
			{	//выводим до термнатора
				printf("%c", *ptr++);
			}
		}
		return 0;
}
