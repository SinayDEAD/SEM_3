Ибрагимова София

Группа: 208

Задание: P03 "Калькулятор"

Важное: Программа основана на стеке и ОПЗ/RPN

На вход в обоих папках поступает выражение, считывается оно до пробела, отличия, что в папке ForUser
Это уже готовый калькулятор, в том числе для переменных, во второй же папке с помощью makefile и команды
"cat test|./script" можно увидеть где и как выдаст ошибки, а где нужно считает

Разрешены операции *,/,+,-,()

Разрешены любые переменные состоящие из букв латинского алфавита, НО состоящие только из них (заглавных и строчных)
Разрешены отрицательные числа, но операция унарного минуса нет, поэтому замена переменной "а" на -23 возможна, а замена
"-a" на -23 или 23 нет.

Есть два уровня проверки ошибок, при вводе и при подсчёте, это можно увидеть на тестах, или же при вводе примера
 "a/a" и каменной а на 0 второй уровень проверки выдаст неверную замену или операцию

Можно ввести 012, 12, 12. , .12, но нельзя . Или 12 12 или 12.12.12

Важно:
В архиве две папки,
Первая полный калькулятор и его можно просто запустить и при желании запускать не выходя с примером "a-2" 
Прожигая каждый раз подсчёт видеть предыдущие значение при подставлении переменной 
Например 
Вводя
A-2
Потом
1
Потом
a
Потом
3
Выдаст -1
И можно выбрать либо выход, со всем удалением либо заново ввести переменную и видеть предыдущие значения

Вторая же это пример для теста тоже с makefile.
Главное отличие, что в этой программе нет возможности считать константы и тд
То есть, можно вводить только числа, (потребовалось, чтобы показать все возможные ошибки И так далее)

Также при ошибке будет указываться на какой она позиции при вводе и перед чем например при вводе
2+2)
Выдаст 
Ошибка символом ) под номером 3

// Всё, что связано с Полисом

extern void ErrorsInRPN(void);
extern void PlusMinusImport(void);
extern void MultiplicationDivisionImportance(void);
extern void PlusMinusImportAfterSmth(void);
extern void Upor(void);
extern void Perem(void);
extern void Numberr(void);
extern void NumberrAfterDot(void);
extern void Peremen(void);
extern void Sdvig(int t);

// Меню

void PrintMenu(void);
int GetVariant(int count);

//Всё, что нужно для перегонки из файла
//потом в стек

int SizeOfFile(FILE *f);
size_t PotstanovkaPeremen(const char *src, char *dst, const char *old, const char *new);
void Schet(char *src);
char *Vvod(int *len);

// Стек

struct Stek{
    struct Stek *next_p;
    double d;
};
struct Stek *Stek_new(double d){
    struct Stek *Stek_p;
    Stek_p = malloc(sizeof(struct Stek));
    Stek_p -> next_p = NULL;
    Stek_p -> d = d;
    return Stek_p;
}
struct Stek *base_p = NULL;
void ProverkaStroki(char *line_p);
void VvodVStekIPechat(void);
int  StekPop(double *d_p);
void StekPush(double d);
int UnaryPop(double *x_p);
int BinaryPop(double *x_p, double *y_p);
int Vsiatie(char *token_p);
int VsiatieDouble(char *token_p, double *d);
int Pluss(void);
int Minuss(void);
int Multiplication(void);
int Division(void);
int Deletion(void);
void StekDeletion(struct Stek *Stek_p);
//объявления, что требуется
char slovo;
int position = 0;
int file_size = 0; //можно если, что расширить, если нужно
int len1, len2;
FILE *file;