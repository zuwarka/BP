#define _CRT_SECURE_NO_WARNINGS
#define N 100
#define NO 0
#define YES 1
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

typedef struct
{ //1 файл
	int numroute; //номер линии ПИШЕМ В ФАЙЛ
	char type[N]; //автобус, трамвай, метро и т.п. ПИШЕМ В ФАЙЛ
	char schedule[N]; //время работы маршрута ПИШЕМ В ФАЙЛ
	//unsigned int lineid; //уникальный идентификатор линии
} TRANS;

typedef struct
{ //2 файл
	char name[N]; //название станции ПИШЕМ В ФАЙЛ
	//unsigned int id; //уникальный идентификатор станции
	//unsigned int lineid; //идентификатор линии, к которой относится станция
} STOPS;

typedef struct
{	//структура для создания массива (выполнение 4 и 5 пунктов)
	char name[N]; //имя станции
	int* trans; //указатель
	int trans_size; //количество маршрутов, относящиеся к данной станции
} Stops_Trans_t;

typedef struct STOPS_Node
{	//узел двусвязного списка станций
	struct STOPS_Node* next; //указатель на следующий элемент
	struct STOPS_Node* prev; //указатель на предыдущий элемент
	STOPS St; //данные о станции: имя, айдишники
} STOPS_Node_t;

typedef struct STOPS_List
{	//пустая структура, хранящая указатели на голову и хвост списка станций
	STOPS_Node_t* head;
	STOPS_Node_t* tail;
	int size; //размер списка (возможно пригодится)
} STOPS_List_t;

typedef struct Node
{	//узел общего списка данных о маршруте: характеристики транспорта, список станций к нему относящийся
	struct Node* next;
	struct Node* prev;
	TRANS Tr;
	STOPS_List_t* St_l_t;
} Node_t;

typedef struct List_t
{	//пустая структура, хранящая указатели на голову и хвост общего списка данных о маршруте
	Node_t* head;
	Node_t* tail;
	int size;
} List_t;

					/* -- Функци инициализации списков -- */

void init_Sl(STOPS_List_t* Sl)
{	//создание списка станций
	Sl->head = NULL;
	Sl->tail = NULL;
	Sl->size = 0;
}

void init(List_t* l)
{	//создание общего списка данных
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
}

					/* --- Начало основных функций --- */

Stops_Trans_t** comparison_and_add(int num_trans, Stops_Trans_t** stt, int size_stt, STOPS_Node_t* curr_SNt, int* check)
{	//проверяем и добавляем маршруты и станции
	//check нужен для того, чтобы узнать, увеличился наш список по станциям или нет
	int flag = 0;
	*check = 0; //меняем содержимое переменной
	int i = 0;
	while (i != size_stt)
	{	//пока счетчик не равен количеству элементов в главном массиве - обход главного массива
		int j = 0;
		for (j; curr_SNt->St.name[j] != '\0'; j++)
		{	
			if (stt[i]->name[j] == curr_SNt->St.name[j])
			{	//сравниваем введенную станцию с имеющейся в списке
				continue;
			}
			else
				break; //нашли
		}
		if (stt[i]->name[j] == curr_SNt->St.name[j])
		{	//наши станции равны

			stt[i]->trans_size = stt[i]->trans_size + 1; //следовательно, станция пересадки, увеличиваем количество транспортов

			int* new_trans = (int*)malloc(stt[i]->trans_size * sizeof(int)); //создаем буфер - массив количества маршрутов, проходящих через эту станцию
			new_trans[stt[i]->trans_size - 1] = num_trans; //добавили в буфер номер маршрута

			for (int k = 0; k < stt[i]->trans_size - 1; k++)
			{
				new_trans[k] = stt[i]->trans[k]; //перенесли в буфер эти маршруты
			}
			free(stt[i]->trans); //очистили старый
			stt[i]->trans = new_trans; //обновили
			return stt; //вернули главный обновленный массив
		}
		i++;
	}
	*check = 1; //увеличиваем переменные размера массива
	size_stt++;
	Stops_Trans_t** new_stt = (Stops_Trans_t**)malloc(size_stt * sizeof(Stops_Trans_t*)); //буфер главного массива
	Stops_Trans_t* new_stt_el = (Stops_Trans_t*)malloc(sizeof(Stops_Trans_t)); //то же, что выше
	for (i = 0; curr_SNt->St.name[i] != '\0'; i++) //переписка данных в буфер
		new_stt_el->name[i] = curr_SNt->St.name[i];
	new_stt_el->name[i] = '\0';
	new_stt_el->trans_size = 1;
	int* new_trans = (int*)malloc(new_stt_el->trans_size * sizeof(int));
	new_stt_el->trans = new_trans;
	new_stt_el->trans[0] = num_trans;
	new_stt[size_stt - 1] = new_stt_el;
	for (i = 0; i < size_stt - 1; i++)
	{
		new_stt[i] = stt[i];
	}
	free(stt); //очистка главного массива
	return new_stt; //возвращение "буфера" - дополненного главного списка
	//можно было бы использовать realloc, но это было труднореализуемо :(
}

Stops_Trans_t** form_stt(List_t* l, int* size_stt) //данная функция вернет указатель на указатель на тип Stops_Trans_t (массив указателей)
{	//создаем массив на основе имеющегося списка
	Node_t* curr = l->head; //берём текущий элемент ГЛАВНОГО списка
	Stops_Trans_t** stt = NULL; //станция, в которой хранятся маршруты
	int i = 0; //стчечик колтчества элементов в stt

	while (curr != NULL) //цикл пока не пройдём полностью ГЛАВНЫЙ список
	{
		STOPS_Node_t* curr_SNt = curr->St_l_t->head; //взяли список станций из узла главного списка
		while (curr_SNt != NULL) //идём по узлу станций
		{
			STOPS curr_name = curr_SNt->St; //забрали название станции
			if (!stt) //если ещё нет станций в массиве структур
			{
				i++;
				stt = (Stops_Trans_t**)malloc(i * sizeof(Stops_Trans_t*)); //создали массив на 1 элемент
				Stops_Trans_t* stt_el = (Stops_Trans_t*)malloc(sizeof(Stops_Trans_t)); //элемент главного массива (имя станции + маршруты)
				int j = 0; //счётчик для записи остановки

				while (curr_name.name[j] != '\0')
				{	//пишем им станции в элемент массива
					stt_el->name[j] = curr_name.name[j];
					j++;
				}
				stt_el->name[j] = '\0';

				stt_el->trans_size = 1; //данная станция принадлежит одному маршруту
				stt_el->trans = (int*)malloc(stt_el->trans_size * sizeof(int)); //массив количества маршрутов
				stt_el->trans[stt_el->trans_size - 1] = curr->Tr.numroute; //записали номер
				stt[0] = stt_el;
			}
			else
			{
				int check = 0;
				stt = comparison_and_add(curr->Tr.numroute, stt, i, curr_SNt, &check); //сравнение и добавление
				//номер маршрута из главного списка, массив, кол-во элементов в нем, список станций, адрес check
				if (check)
					i++; //идем дальше по циклу
			}
			curr_SNt = curr_SNt->next;
		}
		curr = curr->next;
	}
	*size_stt = i;
	return stt; //возвращаем главный массив
}

void dell_stt(Stops_Trans_t** stt, int size_stt)
{	//удаление главного массива
	int i = 0;
	while (i != size_stt)
	{
		free(stt[i]->trans);
		free(stt[i]);
		i++;
	}
	free(stt);
}

void print_transfer(List_t* l)
{	//функция вывода станций пересадок, 6
	int size_stt = 0; //размер главного массива
	Stops_Trans_t** stt = form_stt(l, &size_stt); //создаем главный массив со станциями
	printf("\nСтанции пересадок:\n");
	for (int i = 0; i < size_stt; i++) //перебор главного массива
	{
		if (stt[i]->trans_size > 1) //если количество маршрутов у станции больше одного, то перед нами станция пересадки
		{
			printf("Станция: %s\n", stt[i]->name);
			printf("Маршруты данной станции: ");
			for (int j = 0; j < stt[i]->trans_size; j++)
			{
				printf("%d ", stt[i]->trans[j]);
			}
			printf("\n");
		}
	}
	dell_stt(stt, size_stt);
}

void push_back(List_t* l, TRANS tr, STOPS_List_t* st_l)
{	//функция добавления элемента в конец списка данных
	Node_t* curr = (Node_t*)malloc(sizeof(Node_t)); //выделяем память под содержание узла данных о маршруте
	curr->Tr = tr; //передаем выделенному узлу транспортные характеристики
	curr->St_l_t = st_l; //передаем ему же список станций данного маршрута

	if (l->head == NULL)
	{	//если голова нулевая, то есть, список пуст, то
		curr->prev = NULL; //предыдущего элемента нет
		curr->next = NULL; //следующего тоже
		l->head = curr; //голова указывает на только то созданный узел данных
		l->tail = curr; //хвост тоже
	}
	else
	{	//иначе у нас уже имеется список
		curr->next = NULL; //следующий узел нулевой
		curr->prev = l->tail; //хвост ставим на созданный узел
		curr->prev->next = curr; //указатель на следующий элемент предыдущего узла присваиваем на созданный узел
		l->tail = curr; //хвост указывает на созданный элемент
	}
	l->size++; //увеличиваем размер
}

void push_back_Sl(STOPS_List_t* Sl, STOPS st)
{	//функция вставки станции в список станции (процессы аналогичны таковым в push_back)
	STOPS_Node_t* curr = (STOPS_Node_t*)malloc(sizeof(STOPS_Node_t));
	curr->St = st;

	if (Sl->head == NULL)
	{
		curr->prev = NULL;
		curr->next = NULL;
		Sl->head = curr;
		Sl->tail = curr;
	}
	else
	{
		curr->next = NULL;
		curr->prev = Sl->tail;
		curr->prev->next = curr;
		Sl->tail = curr;
	}
	Sl->size++;
}

void delete_Sl(STOPS_List_t* Sl)
{	//функция удаления списка станций
	STOPS_Node_t* curr = Sl->head; //созданный "станционный" узел указывает на голову списка станций
	STOPS_Node_t* prev_curr; //"предыдущий" указатель

	while (curr != NULL)
	{	//цикл до нуля (до конца списка)
		prev_curr = curr; //предудщему указателю присваиваем значение настоящего,
		curr = curr->next; //а настоящему - следующий
		free(prev_curr); //"настоящий" элемент стираем
	}
}

void delete_l(List_t* l)
{	//функция удаления общего списка данных (процессы аналогичны таковым у delete_Sl)
	Node_t* curr = l->head;
	Node_t* prev_curr;

	while (curr != NULL)
	{
		prev_curr = curr;
		curr = curr->next;
		delete_Sl(prev_curr->St_l_t);
		free(prev_curr->St_l_t);
		prev_curr->St_l_t = NULL;
		free(prev_curr);
	}
}

void erase(List_t* l, Node_t* n)
{	//функция стирания необходимых данных
	if (!l->size)
		return; //на случай, если в общем списке нет никаких узлов, то функция erase не проходится
	if (
		((n->Tr.numroute == l->head->Tr.numroute) && (n->Tr.numroute == l->tail->Tr.numroute))
		&& (!strcmp(l->head->Tr.type, n->Tr.type)) && (!strcmp(l->tail->Tr.type, n->Tr.type))
		) //если введенный на удаление узел - единственный в списке, то
	{
		delete_Sl(l->head->St_l_t); //удаляем список станций
		free(l->head->St_l_t); //освобождаем память на него
		free(l->head);
		init(l); //заново инициализируем список общих данных, но уже пустой
		return;
	}

	if ((n->Tr.numroute == l->head->Tr.numroute) && (!strcmp(l->head->Tr.type, n->Tr.type)))
	{	//если введенный узел - первый элемент в общем списке
		Node_t* new_head = l->head->next; //создаем новую голову
		new_head->prev = NULL;
		delete_Sl(l->head->St_l_t);
		free(l->head->St_l_t);
		free(l->head);
		l->head = new_head; //переприсваиваем голову тму элементу, который был после нее
		l->size--; //уменьшаем размер
		return;
	}

	if ((n->Tr.numroute == l->tail->Tr.numroute) && (!strcmp(l->tail->Tr.type, n->Tr.type)))
	{	//если веденный узел - хвост общего списка - процессы почти аналогичны таковым в предыдущем условии
		Node_t* new_tail = l->tail->prev;
		new_tail->next = NULL;
		delete_Sl(l->tail->St_l_t);
		free(l->tail->St_l_t);
		free(l->tail);
		l->tail = new_tail;
		l->size--;
		return;
	}
	//иначе перед нами "срединный" узел общего списка 
	Node_t* curr = l->head; //берем указатель на начало общего списка
	int flag = NO; //признак присутствия введенных данных в общем списке данных
	while (curr != NULL) //цикл до конца списка
	{
		if (curr->Tr.numroute == n->Tr.numroute)
		{	//если нашли номер
			if (!strcmp(curr->Tr.type, n->Tr.type))
			{	//если нашли тип транспорта
				curr->prev->next = curr->next; //переприсваиваем указатели соседних с узлом элементов 
				curr->next->prev = curr->prev;

				delete_Sl(curr->St_l_t); //удаляем список станций
				free(curr->St_l_t); //освобождаем память
				free(curr);
				flag = YES; //нашли узел
				l->size--; //уменьшаем размер
				break; //досрочно выходим из цикла
			}
		}
		curr = curr->next; //шаг цикла
	}
	if (flag == NO)
	{
		printf("Такого маршрута не существует!\n");
	}
}

void inputting(List_t* l)
{	//функция ввода новых данных о маршруте, 1
	TRANS transport;
	int lineid = l->size;
	int stopid = l->tail->St_l_t->size;
	STOPS_Node_t* curr_st = (STOPS_Node_t*)malloc(sizeof(STOPS_Node_t));
	STOPS_Node_t* new_curr_st = NULL;

	printf("\nВвод характеристик данного маршрута\n");
	printf("Введите номер маршрута: ");
	scanf("%d", &transport.numroute);

	printf("\nВведите тип транспорта: ");
	scanf("%s", transport.type);

	printf("\nВведите время работы маршрута (от-до): ");
	scanf("%s", transport.schedule);

	char Exit[] = "0", string[N];

	printf("\nВвод остановок (станций) данного маршрута (введите 0 для завершения ввода)\n");
	printf("Введите названия остановок (станций):\n");

	STOPS_List_t* Sl = (STOPS_List_t*)malloc(sizeof(STOPS_List_t));
	init_Sl(Sl); //создаем список под станции
	int check = 0;
	while (1) //ввод остановок, пока пользователь не напишет "0"
	{
		STOPS st;;
		gets(string);
		if (!check)
		{
			check = 1;
			continue;
		}
		if (strcmp(string, Exit) == 0) {
			break;
		}
		strcpy(st.name, string); //копируем имя станции
		push_back_Sl(Sl, st); //заносим данные в список станций
	}
	push_back(l, transport, Sl); //заносим данные в общий список маршрута
}

void deldata(List_t* l)
{	//функция удаления данных по номеру и типу, 3
	setlocale(LC_ALL, "Rus");
	int find_numroute;
	char find_type[N];

	printf("Введите номер маршрута: ");
	scanf("%d", &find_numroute);
	printf("\nВведите тип транспорта: ");
	scanf("%s", find_type);

	Node_t* erase_el = (Node_t*)malloc(sizeof(Node_t)); //создаем буфер под введенные данные
	erase_el->Tr.numroute = find_numroute; //пишем туда введенное значение номера
	int i = 0;
	for (i; find_type[i] != '\0'; i++) //пишем введенное значение типа, пока не встретм нуль-терминатор строки
		erase_el->Tr.type[i] = find_type[i];
	erase_el->Tr.type[i] = '\0';
	erase(l, erase_el); //идем в функцию затирки введенного элемента
	free(erase_el); //очищаем буфер
}

int menu(void)
{	//меню выбора
	int c = 0;
	while ((c < '0' || c > '7') && c != 27)
	{
		printf("0 : Выход\n");
		printf("1 : Ввести новые данные\n");
		printf("2 : Вывести все маршруты со станциями\n");
		printf("3 : Удалить данные\n");
		printf("4 : Редактирование данных\n");
		printf("5 : Сохранить введенные данные в файлах\n");
		printf("6 : Вывести станции пересадок\n");
		printf("7 : Подбор оптимального маршрура\n");
		printf("> ");
		c = _getch();
		printf("%c\n", c);
	}
	return c;
}

void check_erase(char* str)
{	//функция "удаления" символов escape-последовательностей, которые будут мешать в дальнейших операциях
	char* del_str = strstr(str, "\n");
	del_str[0] = '\0';
}

void check_data(List_t* l)
{	//функция проверки наличия данных в файлах перед началом работы программы
	FILE* f1 = fopen("transport.txt", "r");
	if (!f1)
		exit(1); //ошибка открытия файла
	FILE* f2 = fopen("stations.txt", "r");
	if (!f2)
		exit(1); //ошибка открытия файла
	while (!feof(f1))
	{
		TRANS tr;
		char buf[N] = "0";
		char* flag;
		flag = fgets(buf, N, f1);
		if (!flag)
			break;
		tr.numroute = atoi(buf); //выделяем из строки взятой строки только цифру (номер маршрута) без escape-последовательностей

		flag = fgets(tr.type, N, f1);
		if (!flag)
			exit(1);
		check_erase(tr.type); //выделяем из строки только тип

		flag = fgets(tr.schedule, N, f1);
		if (!flag)
			exit(1);
		check_erase(tr.schedule); //только время работы

		STOPS_List_t* Sl = (STOPS_List_t*)malloc(sizeof(STOPS_List_t));
		init_Sl(Sl); //создаем список под станции

		while (1)
		{
			int i = 0;
			flag = fgets(buf, N, f2);

			if ((buf[0] == '\n') || (!flag))
				break; //мы дошли до разграничительной строки данного маршрута или попалась нулевая строка
			STOPS st;
			for (i; buf[i] != '\n'; i++)
				st.name[i] = buf[i];
			st.name[i] = '\0';
			push_back_Sl(Sl, st);
		}

		push_back(l, tr, Sl); //занесли данные в общий список

		flag = fgets(buf, N, f1); //еще раз берем строку, последнюю
		if (!flag) //если она нулевая, то выходим досрочно из цикла
			break;
		if (*flag != '\n') //если она не равна пустой строке, то досрочно выходим из цикла, иначе продолжаем цикл 
			break;
	}

	fclose(f1);
	fclose(f2);
}

Node_t* find(List_t* l, Node_t* n)
{	//функция поиска, возвращающая значение (указатель на тип) Node_t
	Node_t* curr = l->head;

	while (curr != NULL)
	{
		if (curr->Tr.numroute == n->Tr.numroute)
		{
			if (!strcmp(curr->Tr.type, n->Tr.type))
			{
				return curr; //нужный элемент найден, возвращаем указатель на него
			}
		}
		curr = curr->next;
	}

	return NULL; //иначе не найден
}

void set_data_node(Node_t* n, STOPS_List_t* Sl, TRANS ts)
{	//функция сохранения введенных изменений при редактировании
	delete_Sl(n->St_l_t); //удаляем старый список станций
	free(n->St_l_t);
	n->St_l_t = Sl; //заносим новый
	n->Tr = ts; //заносим новые характеристики
}

void editing(List_t* l)
{	//функция редактирования данных, 3
	setlocale(LC_ALL, "Rus");
	int find_numroute;
	char find_type[N];

	printf("Введите номер маршрута: ");
	scanf("%d", &find_numroute);
	printf("\nВведите тип транспорта: ");
	scanf("%s", &find_type);

	Node_t* find_el = (Node_t*)malloc(sizeof(Node_t)); //выделяем буфер для поискового узла - процессы аналогичны таковым в deldata
	find_el->Tr.numroute = find_numroute;
	int i = 0;
	for (i; find_type[i] != '\0'; i++)
		find_el->Tr.type[i] = find_type[i];
	find_el->Tr.type[i] = '\0';
	Node_t* new_find_el = NULL;
	new_find_el = find(l, find_el); //ищем узел в списке по заданным характеристикам, присвоив значение узла общего списка выделенному выше узлу
	free(find_el);
	if (!new_find_el)
	{
		printf("Такого маршрута не существует!\n");
		return;
	}

	TRANS transport;
	printf("Введите номер маршрута: ");
	scanf("%d", &transport.numroute);
	printf("\nВведите тип транспорта: ");
	scanf("%s", &transport.type);
	printf("\nВведите время работы маршрута (дни работы, время с и до): ");
	scanf("%s", &transport.schedule);


	char Exit[] = "0", string[N];
	printf("\nВведите названия остановок (станций) или 0 для завершения ввода:\n");

	STOPS_List_t* Sl = (STOPS_List_t*)malloc(sizeof(STOPS_List_t));
	init_Sl(Sl);
	int check = 0;
	while (1) //ввод остановок, пока пользователь не напишет "0"
	{
		STOPS st;
		gets(string);
		if (!check)
		{
			check = 1;
			continue;
		}
		if (strcmp(string, Exit) == 0) {
			break;
		}
		strcpy(st.name, string);
		push_back_Sl(Sl, st);

	}
	//выше описаны процессы, аналогичные таковым в inputting
	set_data_node(new_find_el, Sl, transport); //сохраняем новые данные для узла
}

void print(List_t* l)
{	//функция вывода на консоль меющихся данных, 2
	Node_t* curr = l->head;

	while (curr != NULL)
	{
		printf("\n---------------------------------------\n");

		printf("Numroute: %d\n", curr->Tr.numroute);
		printf("Type: %s\n", curr->Tr.type);
		printf("Schedule: %s\n", curr->Tr.schedule);
		printf("Stations:\n");
		STOPS_Node_t* curr_ost = curr->St_l_t->head;
		while (curr_ost != NULL)
		{
			printf("%s\n", curr_ost->St.name);
			curr_ost = curr_ost->next;
		}
		curr = curr->next;
		printf("\n---------------------------------------\n");
	}
}

void saving(List_t* l)
{	//функция сохранения списка в файлах, 5
	FILE* trans, * stop;
	Node_t* curr = l->head;
	trans = fopen("transport.txt", "w");
	if (!trans)
		exit(1); //ошибка открытия файла
	stop = fopen("stations.txt", "w");
	if (!stop)
		exit(1); //ошибка открытия файла

	while (curr != NULL)
	{
		fprintf(trans, "%d\n", curr->Tr.numroute);
		fprintf(trans, "%s\n", curr->Tr.type);
		fprintf(trans, "%s\n", curr->Tr.schedule);
		fprintf(trans, "\n");
		STOPS_Node_t* curr_st = curr->St_l_t->head;
		while (curr_st != NULL)
		{
			fprintf(stop, "%s\n", curr_st->St.name);
			curr_st = curr_st->next;
		}
		fprintf(stop, "\n");
		curr = curr->next;
	}
	fclose(trans);
	fclose(stop);
}

Stops_Trans_t* stations_or_not(Stops_Trans_t** stt, char* str, int size_stt) //главный массив, введенная станция, размер главного массива
{	//функция вернет указатель типа Stops_Trans_t*
	int i = 0;
	while (i != size_stt) //счетчик по гланому массиву
	{
		int j = 0;
		for (j; stt[i]->name[j] != '\0'; j++)
		{
			if (str[j] == stt[i]->name[j])
				continue; //введенная станция соответствует таковой в главном массиве
			else
				break;
		}
		if (str[j] == stt[i]->name[j])
		{
			return stt[i]; //вовзращаем соответствующий элемент главного массива
		}
		i++;
	}
	return NULL;
}

int check_one(Stops_Trans_t** stt, char* str1, char* str2, int size_stt) //смотрим, есть ли станции вообще и есть ли прямой маршрут по ним
{	//возвратим 0, если нет станций, 1 если один маршрут и выведем его тут
	Stops_Trans_t* first = NULL;
	Stops_Trans_t* second = NULL;
	first = stations_or_not(stt, str1, size_stt); //для начальной станции
	second = stations_or_not(stt, str2, size_stt); //для конечной станции
	if (!first || !second)
		return 0; //на случай ошибки или если станцию не нашли
	//станции "вернулись", начинаем вложенные итерационные циклы
	for (int i = 0; i < first->trans_size; i++) //первый цикл - пока шаг меньше количества транспортов у начальной станции
	{
		for (int j = 0; j < second->trans_size; j++) //второй цикл - пока шаг меньше количества транспортов у введенной станции
		{
			if (first->trans[i] == second->trans[j]) //если в массиве количества маршрутов присутсвуют одинаковые номера маршрутов для обех станций
			{
				printf("\n%d: %s -> %s\n", first->trans[i], str1, str2); //выводим "начальная станция -> пересадочная станция"
				return 1; //функция исполнена
			}
		}
	}

	int check1 = NO;
	int check2 = NO;
	int i = 0;
	while (i != size_stt) //обход главного массива
	{	//начинаем вложенные циклы
		for (int j = 0; j < first->trans_size; j++)
		{	//первый цикл - пока шаг меньше количества транспортов у начальной станции
			for (int k = 0; k < second->trans_size; k++) //второй цикл - пока шаг меньше количества транспортов у введенной станции
			{
				for (int n = 0; n < stt[i]->trans_size; n++) //третий цикл - пока шаг меньше количества станций в главном массиве
				{
					if (first->trans[j] == stt[i]->trans[n])
						check1 = YES; //станция пересадки как для маршрута начальной станции
					if (second->trans[k] == stt[i]->trans[n])
						check2 = YES; //так и для маршрута конечной
				}
				if (check1 == YES && check2 == YES)
				{
					printf("\n%d: %s -> %s\n%d: %s -> %s\n", first->trans[j], str1, stt[i]->name, second->trans[k], stt[i]->name, str2);
					return 2; //нашли оптимальный путь - задание выполнено
				}
				check1 = check2 = NO;

			}
		}
		i++;
	}
	return 3; //иначе оптимального пути между введенными станциями нет
}

void opt_tr(List_t* l)
{	//Подбор маршрута согласно заданию, 7
	char str1[N];
	char str2[N];
	printf("Введите начальную станцию: ");
	scanf("%s", str1);
	printf("Введите конечную станцию: ");
	scanf("%s", str2);

	int size_stt = 0; //размер будущего массива станций станций и маршрутов к ним относящиеся
	Stops_Trans_t** stt = form_stt(l, &size_stt); //создаем главный массив элементов Stops_Trans_t* (имя + маршруты), берем главный список и адрес размера

	int check = check_one(stt, str1, str2, size_stt); //функция принимает введенные станции, а также главный массив и его размер, найденный выше
	if (!check)
	{
		printf("\nОшибка, нет станций!\n");
		return;
	}
	if ((check == 1) || (check == 2))
		return;
	if (check == 3)
		printf("Невозможно подобрать маршрут с не более, чем одной пересадкой. Введите команду еще раз...\n");
	dell_stt(stt, size_stt); //удаляем созданный массив
}

int main(void) //главная функция, оперирующая всем процессом
{
	List_t* l = (List_t*)malloc(sizeof(List_t)); //выделяем память под список

	init(l); //инициализируем его
	setlocale(LC_ALL, "Rus");
	check_data(l); //проверяем наличе данных в файлах
	int Selection;
	while ((Selection = menu()) != '0')
	{
		switch (Selection)
		{
		case '1':
			inputting(l);
			break;
		case '2':
			print(l);
			break;
		case '3':
			deldata(l);
			break;
		case '4':
			editing(l);
			break;
		case '5':
			saving(l);
			break;
		case '6':
			print_transfer(l);
			break;
		case '7':
			opt_tr(l);
			break;
		default:
			break;
		}
	}
	delete_l(l); //по завершении программы удаляем список
	free(l); //и очищаем память под него выделенную
	return 0;
}
