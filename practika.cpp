#include "stdafx.h" // подключение библиотек
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>
#include <windows.h> 

using namespace std;
using namespace System;
using namespace System::IO;

#define ENTER 13 // обозначение кнопок 
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

const char dan[8][70] = {
    "Какая максимальная продолжительность среди всех фильмов?",
    "Количество фильмов жанра \"драма\"?                       ",
    "Алфавитный порядок фильмов                              ",
    "Какой фильм вышел раньше всех?                          ",
    "Обратный алфавитный список фильмов                      ",
    "Фильмы из X страны с продолжительностью > X минут       ",
    "Диаграмма                                               ",                                    
    "Выход                                                   ",
}; //задание пунктов меню
const char BlankLine[] = "                                                        ";

struct z {
    char name[40]; //название фильма
    char janr[20]; //жанр фильма
    char country[20]; //страна производства
    int god; //год выпуска
    int prod; // продолжительность
}; //шаблон структуры для исходных данных

struct sp {
    char name[40]; //название фильма
    int prod; //продолжительность
    struct sp* sled; //ссылка на следующий фильм
    struct sp* pred; //ссылка на предыдущий фильм
}; //шаблон структуры для списка фильмов

struct sp* spisok = nullptr;

int menu(int); //указание шаблонов используемых функций
void maxim(struct z*, int);
void kolvo(struct z*, int);
void alfalist(struct z*, int);
void reverse_alfalist(struct z*, int);
void first(struct z*, int);
void strana(struct z*, int);
void diagram(struct z*, int);
void vstavka(struct z*, int, char*);
void make_list(struct z*, int);
void save_to_file(const char*);

int main(array<System::String ^> ^args) {
    int i, n, NC;
    FILE *in;
    struct z *films;

    setlocale(LC_CTYPE, "Russian"); //поддержка русского языка
    Console::CursorVisible::set(false); //отключение видимости курсора
    Console::BufferHeight = Console::WindowHeight;
    Console::BufferWidth = Console::WindowWidth;

    if ((in = fopen("Films.txt", "r")) == NULL) {
        printf("\nФайл Films.txt не открыт!");
        getch();
        exit(1);
    }

    fscanf(in, "%d", &NC); //считывание числа строк в исходном файле
    films = (struct z*)malloc(NC * sizeof(struct z)); //выделение памяти

    for (i = 0; i < NC; i++) //чтение из файла
        fscanf(in, "%s%s%s%d%d", films[i].name,
            films[i].janr, films[i].country,
            &films[i].god, &films[i].prod);

    printf("Название                                 Жанр                 Страна                 Год выпуска   Продолжительность");
    printf("                                                                                   \n"); 
    for (i = 0; i < NC; i++) //вывод данных на экран
        printf("\n%-40s %-20s %-20s %10d %15d",
            films[i].name, films[i].janr,
            films[i].country, films[i].god,
            films[i].prod);
    _getch();

    while (1) { //создание области меню
        Console::ForegroundColor = ConsoleColor::Green;
        Console::BackgroundColor = ConsoleColor::Black;
        Console::Clear();
        Console::ForegroundColor = ConsoleColor::Green;
        Console::BackgroundColor = ConsoleColor::Black;
        Console::CursorLeft = 10; //точка начала меню
        Console::CursorTop = 4;
        printf(BlankLine);

        for (i = 0; i < 8; i++) { //вывод вопросов
            Console::CursorLeft = 10;
            Console::CursorTop = i + 5;
            printf("%s", dan[i]);
        }
        Console::CursorLeft = 10; //точка конца меню
        Console::CursorTop = 12;
        printf(BlankLine);
        n = menu(8); //выбор вопроса в меню
        switch (n) {
            case 1: maxim(films, NC); break;
            case 2: kolvo(films, NC); break;
            case 3: alfalist(films, NC); break;
            case 4: first(films, NC); break;
            case 5: reverse_alfalist(films, NC); break;
            case 6: strana(films, NC); break;
            case 7: diagram(films, NC); break;
            case 8: save_to_file("output.txt"); exit(0);
        }
    } //конец цикла while(1)

    return 0;
} //конец main()

int menu(int n) {
    int y1 = 0, y2 = n - 1;
    char c = 1;
    while (c != ESC) {
        switch (c) {
            case DOWN: y2 = y1; y1++; break;
            case UP: y2 = y1; y1--; break;
            case ENTER: return y1 + 1;
            case HOME: y2 = y1; y1 = 0; break;
            case END: y2 = y1; y1 = n - 1; break;
        }
        if (y1 > n - 1) { y2 = n - 1; y1 = 0; }
        if (y1 < 0) { y2 = 0; y1 = n - 1; }
        Console::ForegroundColor = ConsoleColor::White;
        Console::BackgroundColor = ConsoleColor::Blue;
        Console::CursorLeft = 10;
        Console::CursorTop = y1 + 5;
        printf("%s", dan[y1]);
        Console::ForegroundColor = ConsoleColor::Green;
        Console::BackgroundColor = ConsoleColor::Black;
        Console::CursorLeft = 10;
        Console::CursorTop = y2 + 5;
        printf("%s", dan[y2]);
        c = _getch();
    }
    exit(0);
}

void maxim(struct z* film, int NC) { //функция поиска максимальной продолжительности фильма
    int i = 0;
    struct z best;
    strcpy(best.name, film[0].name); 
    best.prod = film[0].prod; //в переменную структуры записывается нулевой элемент массива структур
    for (i = 1; i < NC; i++)
        if (film[i].prod > best.prod) {
            strcpy(best.name, film[i].name);
            best.prod = film[i].prod;
        }
    Console::ForegroundColor = ConsoleColor::White;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::CursorLeft = 10; //область начала вывода ответа
    Console::CursorTop = 15;
    printf("Максимальную продолжительность %d минут", best.prod); //вывод ответа
    Console::CursorLeft = 10;
    Console::CursorTop = 16;
    printf("имеет фильм %s", best.name);
    _getch();
}

void kolvo(struct z* film, int NC) { //функция поиска количества фильмов жанра драма
    int i, k = 0;
    for (i = 0; i < NC; i++)
        if (strcmp(film[i].janr, "драма") == 0) //сравнение жанра текущего фильма с "драма"
            k++;
    Console::ForegroundColor = ConsoleColor::White;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::CursorLeft = 10;
    Console::CursorTop = 15;
    printf("Фильмов \"драма\" всего: %d", k); //вывод количества фильмов жанра "драма" на экран
    _getch();
}

void vstavka(struct z* film, int NC, char* name) {
    int i;
    struct sp* nov, * nt, * z = nullptr;
    for (nt = spisok; nt != nullptr && strcmp(nt->name, name) < 0; z = nt, nt = nt->sled);
	//цикл ищет позицию для вставки нового узла, при этом сохраняется указатель на предыдущий узел (z)
    if (nt && strcmp(nt->name, name) == 0) return;
    nov = (struct sp*)malloc(sizeof(struct sp)); //выделение памяти для нового узла
    strcpy(nov->name, name);
    nov->sled = nt;
    nov->prod = 0;
    for (i = 0; i < NC; i++) //цикл суммирует значения prod всех фильмов с именем name
        if (strcmp(film[i].name, name) == 0)
            nov->prod += film[i].prod;
    if (!z) spisok = nov;
    else z->sled = nov;
}

void make_list(struct z* film, int NC) {
    int i;
    for (i = 0; i < NC; i++) //цикл для вставки каждого элемента из массива `film` в список `spisok`
        vstavka(film, NC, film[i].name);
    struct sp* nt = spisok;
    struct sp* next = nt->sled;
    spisok->pred = nullptr;
    for (; next != nullptr; nt = next, next = next->sled) // Цикл для установки обратных ссылок
        next->pred = nt;
}

void alfalist(struct z* film, int NC) {
    int i;
    struct sp* nt;
    Console::ForegroundColor = ConsoleColor::Green;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    if (!spisok) make_list(film, NC); //если список пуст, вызвать функцию make_list для его заполнения
    printf("\n Алфавитный список фильмов:");
    printf("\n    ===============================\n");
    for (nt = spisok; nt != nullptr; nt = nt->sled) //проход по всему списку spisok
        printf("\n %-20s %10d", nt->name, nt->prod); //вывод названия и продолжительности фильмов
    _getch();
}

void reverse_alfalist(struct z* film, int NC) {
    int i;
    struct sp* nt;
    Console::ForegroundColor = ConsoleColor::Green;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    if (!spisok) make_list(film, NC); //если список пуст, вызвать функцию make_list для его заполнения
    printf("\n Обратный алфавитный список фильмов:");
    printf("\n    ===============================\n");
    for (nt = spisok; nt->sled != nullptr; nt = nt->sled); //проход до конца списка
    for (; nt != nullptr; nt = nt->pred) //проход по списку в обратном порядке
        printf("\n %-20s %10d", nt->name, nt->prod);
    _getch();
}

void first(struct z* film, int NC) {   
    int i;   
    struct z* best = film;   
    for (i = 1; i < NC; i++) //проход по массиву фильмов   
        if (film[i].god < best->god) best = &film[i];   
    Console::ForegroundColor = ConsoleColor::White;   
    Console::BackgroundColor = ConsoleColor::Black;   
    Console::CursorLeft = 10;   
    Console::CursorTop = 15;   
    printf("Фильм, который вышел раньше всех, жанр %s, продолжительность %d минут.",   
        best->janr, best->prod);   
    Console::CursorLeft = 10;   
    Console::CursorTop = 16;   
    printf("называется %s", best->name);   
    Console::CursorLeft = 10;   
    Console::CursorTop = 17;   
    printf("Вышел в %d году", best->god);   
    _getch();   
}

void strana(struct z* film, int NC) {
    int i, duration;
    char country[20];

	setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251); //устанавливаем кодировку ввода
    SetConsoleOutputCP(1251); //устанавливаем кодировку вывода

    //ввод данных от пользователя
    Console::ForegroundColor = ConsoleColor::Green;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    
    Console::CursorLeft = 10;
    Console::CursorTop = 5;
    printf("Введите страну: "); //запрос на ввод страны
    scanf("%s", country);

    Console::CursorLeft = 10;
    Console::CursorTop = 7;
    printf("Введите продолжительность: "); //запрос на ввод продолжительности
    scanf("%d", &duration);

    //вывод отфильтрованных фильмов
    Console::Clear();
    printf("\n Фильмы из %s с продолжительностью > %d минут \n", country, duration);
    printf("\n Фильм                Продолжительность");
    printf("                                    \n");
    for (i = 0; i < NC; i++) {
        if ((strcmp(film[i].country, country) == 0) && (film[i].prod > duration)) {
            printf("\n %-20s %10d ", film[i].name, film[i].prod);
        }
    }
    _getch();
}

void diagram(struct z* film, int NC) {
    struct sp* nt; //объявление указателя на структуру sp
    int len, i, NColor;
    long sum = 0;
    char str1[40]; //объявление массива для хранения названия фильма
    char str2[20]; //объявление массива для хранения процентного соотношения
    System::ConsoleColor Color;
    Console::ForegroundColor = ConsoleColor::Green;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    for (i = 0; i < NC; i++) //проход по массиву фильмов
        sum = sum + film[i].prod;
    if (!spisok)
        for (i = 0; i < NC; i++) //проход по массиву фильмов
            vstavka(film, NC, film[i].name); //вставка фильма в список
    Color = ConsoleColor::Black;
    NColor = 0;
    for (nt = spisok, i = 0; nt != nullptr; nt = nt->sled, i++) { //проход по spisok
        sprintf(str1, "%s", nt->name); //запись названия фильма в str1
        sprintf(str2, "%3.1f%%", (nt->prod * 100. / sum)); //запись процентного соотношения в str2
        Console::ForegroundColor = ConsoleColor::White;
        Console::BackgroundColor = ConsoleColor::Black;
        Console::CursorLeft = 5;
        Console::CursorTop = i + 1;
        printf("%s", str1);
        Console::CursorLeft = 50;
        printf("%s", str2);
        Console::BackgroundColor = ++Color;
        NColor++;
        Console::CursorLeft = 55;
        for (len = 0; len < nt->prod * 100 / sum; len++) //проход по длине процентного соотношения
            printf(" ");
        if (NColor == 14) {
            Color = ConsoleColor::Black;
            NColor = 0;
        }
    }
    _getch();
}

void save_to_file(const char* filename) {
    FILE* out = fopen(filename, "w");
    if (!out) {
        printf("Ошибка открытия файла для записи.\n");
        return;
    }
    struct sp* nt = spisok;
    while (nt) {
        fprintf(out, "%s %d\n", nt->name, nt->prod);
        nt = nt->sled;
    }
    fclose(out);
}
