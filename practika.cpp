#include "stdafx.h" // ����������� ���������
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

#define ENTER 13 // ����������� ������ 
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

const char dan[8][70] = {
    "����� ������������ ����������������� ����� ���� �������?",
    "���������� ������� ����� \"�����\"?                       ",
    "���������� ������� �������                              ",
    "����� ����� ����� ������ ����?                          ",
    "�������� ���������� ������ �������                      ",
    "������ �� X ������ � ������������������ > X �����       ",
    "���������                                               ",                                    
    "�����                                                   ",
}; //������� ������� ����
const char BlankLine[] = "                                                        ";

struct z {
    char name[40]; //�������� ������
    char janr[20]; //���� ������
    char country[20]; //������ ������������
    int god; //��� �������
    int prod; // �����������������
}; //������ ��������� ��� �������� ������

struct sp {
    char name[40]; //�������� ������
    int prod; //�����������������
    struct sp* sled; //������ �� ��������� �����
    struct sp* pred; //������ �� ���������� �����
}; //������ ��������� ��� ������ �������

struct sp* spisok = nullptr;

int menu(int); //�������� �������� ������������ �������
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

    setlocale(LC_CTYPE, "Russian"); //��������� �������� �����
    Console::CursorVisible::set(false); //���������� ��������� �������
    Console::BufferHeight = Console::WindowHeight;
    Console::BufferWidth = Console::WindowWidth;

    if ((in = fopen("Films.txt", "r")) == NULL) {
        printf("\n���� Films.txt �� ������!");
        getch();
        exit(1);
    }

    fscanf(in, "%d", &NC); //���������� ����� ����� � �������� �����
    films = (struct z*)malloc(NC * sizeof(struct z)); //��������� ������

    for (i = 0; i < NC; i++) //������ �� �����
        fscanf(in, "%s%s%s%d%d", films[i].name,
            films[i].janr, films[i].country,
            &films[i].god, &films[i].prod);

    printf("��������                                 ����                 ������                 ��� �������   �����������������");
    printf("                                                                                   \n"); 
    for (i = 0; i < NC; i++) //����� ������ �� �����
        printf("\n%-40s %-20s %-20s %10d %15d",
            films[i].name, films[i].janr,
            films[i].country, films[i].god,
            films[i].prod);
    _getch();

    while (1) { //�������� ������� ����
        Console::ForegroundColor = ConsoleColor::Green;
        Console::BackgroundColor = ConsoleColor::Black;
        Console::Clear();
        Console::ForegroundColor = ConsoleColor::Green;
        Console::BackgroundColor = ConsoleColor::Black;
        Console::CursorLeft = 10; //����� ������ ����
        Console::CursorTop = 4;
        printf(BlankLine);

        for (i = 0; i < 8; i++) { //����� ��������
            Console::CursorLeft = 10;
            Console::CursorTop = i + 5;
            printf("%s", dan[i]);
        }
        Console::CursorLeft = 10; //����� ����� ����
        Console::CursorTop = 12;
        printf(BlankLine);
        n = menu(8); //����� ������� � ����
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
    } //����� ����� while(1)

    return 0;
} //����� main()

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

void maxim(struct z* film, int NC) { //������� ������ ������������ ����������������� ������
    int i = 0;
    struct z best;
    strcpy(best.name, film[0].name); 
    best.prod = film[0].prod; //� ���������� ��������� ������������ ������� ������� ������� ��������
    for (i = 1; i < NC; i++)
        if (film[i].prod > best.prod) {
            strcpy(best.name, film[i].name);
            best.prod = film[i].prod;
        }
    Console::ForegroundColor = ConsoleColor::White;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::CursorLeft = 10; //������� ������ ������ ������
    Console::CursorTop = 15;
    printf("������������ ����������������� %d �����", best.prod); //����� ������
    Console::CursorLeft = 10;
    Console::CursorTop = 16;
    printf("����� ����� %s", best.name);
    _getch();
}

void kolvo(struct z* film, int NC) { //������� ������ ���������� ������� ����� �����
    int i, k = 0;
    for (i = 0; i < NC; i++)
        if (strcmp(film[i].janr, "�����") == 0) //��������� ����� �������� ������ � "�����"
            k++;
    Console::ForegroundColor = ConsoleColor::White;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::CursorLeft = 10;
    Console::CursorTop = 15;
    printf("������� \"�����\" �����: %d", k); //����� ���������� ������� ����� "�����" �� �����
    _getch();
}

void vstavka(struct z* film, int NC, char* name) {
    int i;
    struct sp* nov, * nt, * z = nullptr;
    for (nt = spisok; nt != nullptr && strcmp(nt->name, name) < 0; z = nt, nt = nt->sled);
	//���� ���� ������� ��� ������� ������ ����, ��� ���� ����������� ��������� �� ���������� ���� (z)
    if (nt && strcmp(nt->name, name) == 0) return;
    nov = (struct sp*)malloc(sizeof(struct sp)); //��������� ������ ��� ������ ����
    strcpy(nov->name, name);
    nov->sled = nt;
    nov->prod = 0;
    for (i = 0; i < NC; i++) //���� ��������� �������� prod ���� ������� � ������ name
        if (strcmp(film[i].name, name) == 0)
            nov->prod += film[i].prod;
    if (!z) spisok = nov;
    else z->sled = nov;
}

void make_list(struct z* film, int NC) {
    int i;
    for (i = 0; i < NC; i++) //���� ��� ������� ������� �������� �� ������� `film` � ������ `spisok`
        vstavka(film, NC, film[i].name);
    struct sp* nt = spisok;
    struct sp* next = nt->sled;
    spisok->pred = nullptr;
    for (; next != nullptr; nt = next, next = next->sled) // ���� ��� ��������� �������� ������
        next->pred = nt;
}

void alfalist(struct z* film, int NC) {
    int i;
    struct sp* nt;
    Console::ForegroundColor = ConsoleColor::Green;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    if (!spisok) make_list(film, NC); //���� ������ ����, ������� ������� make_list ��� ��� ����������
    printf("\n ���������� ������ �������:");
    printf("\n    ===============================\n");
    for (nt = spisok; nt != nullptr; nt = nt->sled) //������ �� ����� ������ spisok
        printf("\n %-20s %10d", nt->name, nt->prod); //����� �������� � ����������������� �������
    _getch();
}

void reverse_alfalist(struct z* film, int NC) {
    int i;
    struct sp* nt;
    Console::ForegroundColor = ConsoleColor::Green;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    if (!spisok) make_list(film, NC); //���� ������ ����, ������� ������� make_list ��� ��� ����������
    printf("\n �������� ���������� ������ �������:");
    printf("\n    ===============================\n");
    for (nt = spisok; nt->sled != nullptr; nt = nt->sled); //������ �� ����� ������
    for (; nt != nullptr; nt = nt->pred) //������ �� ������ � �������� �������
        printf("\n %-20s %10d", nt->name, nt->prod);
    _getch();
}

void first(struct z* film, int NC) {   
    int i;   
    struct z* best = film;   
    for (i = 1; i < NC; i++) //������ �� ������� �������   
        if (film[i].god < best->god) best = &film[i];   
    Console::ForegroundColor = ConsoleColor::White;   
    Console::BackgroundColor = ConsoleColor::Black;   
    Console::CursorLeft = 10;   
    Console::CursorTop = 15;   
    printf("�����, ������� ����� ������ ����, ���� %s, ����������������� %d �����.",   
        best->janr, best->prod);   
    Console::CursorLeft = 10;   
    Console::CursorTop = 16;   
    printf("���������� %s", best->name);   
    Console::CursorLeft = 10;   
    Console::CursorTop = 17;   
    printf("����� � %d ����", best->god);   
    _getch();   
}

void strana(struct z* film, int NC) {
    int i, duration;
    char country[20];

	setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251); //������������� ��������� �����
    SetConsoleOutputCP(1251); //������������� ��������� ������

    //���� ������ �� ������������
    Console::ForegroundColor = ConsoleColor::Green;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    
    Console::CursorLeft = 10;
    Console::CursorTop = 5;
    printf("������� ������: "); //������ �� ���� ������
    scanf("%s", country);

    Console::CursorLeft = 10;
    Console::CursorTop = 7;
    printf("������� �����������������: "); //������ �� ���� �����������������
    scanf("%d", &duration);

    //����� ��������������� �������
    Console::Clear();
    printf("\n ������ �� %s � ������������������ > %d ����� \n", country, duration);
    printf("\n �����                �����������������");
    printf("                                    \n");
    for (i = 0; i < NC; i++) {
        if ((strcmp(film[i].country, country) == 0) && (film[i].prod > duration)) {
            printf("\n %-20s %10d ", film[i].name, film[i].prod);
        }
    }
    _getch();
}

void diagram(struct z* film, int NC) {
    struct sp* nt; //���������� ��������� �� ��������� sp
    int len, i, NColor;
    long sum = 0;
    char str1[40]; //���������� ������� ��� �������� �������� ������
    char str2[20]; //���������� ������� ��� �������� ����������� �����������
    System::ConsoleColor Color;
    Console::ForegroundColor = ConsoleColor::Green;
    Console::BackgroundColor = ConsoleColor::Black;
    Console::Clear();
    for (i = 0; i < NC; i++) //������ �� ������� �������
        sum = sum + film[i].prod;
    if (!spisok)
        for (i = 0; i < NC; i++) //������ �� ������� �������
            vstavka(film, NC, film[i].name); //������� ������ � ������
    Color = ConsoleColor::Black;
    NColor = 0;
    for (nt = spisok, i = 0; nt != nullptr; nt = nt->sled, i++) { //������ �� spisok
        sprintf(str1, "%s", nt->name); //������ �������� ������ � str1
        sprintf(str2, "%3.1f%%", (nt->prod * 100. / sum)); //������ ����������� ����������� � str2
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
        for (len = 0; len < nt->prod * 100 / sum; len++) //������ �� ����� ����������� �����������
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
        printf("������ �������� ����� ��� ������.\n");
        return;
    }
    struct sp* nt = spisok;
    while (nt) {
        fprintf(out, "%s %d\n", nt->name, nt->prod);
        nt = nt->sled;
    }
    fclose(out);
}
