#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
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
int GetVariant(int c);
//Всё, что нужно для перегонки из файла
//потом в стек
int SizeOfFile(FILE *f);
size_t PotstanovkaPeremen(const char *src, char *dst, const char *old, const char *new);
void Schet(char *src);
char *Vvod(int *l);
// Стек
struct Stek{
    struct Stek *next;
    double d;
};
struct Stek *Stek_new(double d){
    struct Stek *Stekk;
    Stekk = malloc(sizeof(struct Stek));
    Stekk -> next = NULL;
    Stekk -> d = d;
    return Stekk;
}
struct Stek *base = NULL;
void ProverkaStroki(char *stroka);
void VvodVStekIPechat(void);
int  StekPop(double *d_p);
void StekPush(double d);
int UnaryPop(double *x);
int BinaryPop(double *x, double *y);
int Vsiatie(char *c);
int VsiatieDouble(char *c, double *d);
int Pluss(void);
int Minuss(void);
int Multiplication(void);
int Division(void);
int Deletion(void);
void StekDeletion(struct Stek *Stekk);
//объявления, что требуется
char slovo;
int position = 0;
int file_size = 0; //можно если шо расширить, если нужно
int len1, len2;
FILE *file;

int main(void) {
    //FILE *file;
    file = fopen("rpn.txt", "w");
    slovo = getchar();
    PlusMinusImport();
    fclose(file);
    if (slovo != '\n'){
        printf("\nОшибка символом %c под номером %d\n",slovo,position);
        remove("rpn.txt");
        exit(0);
    }
    FILE *ffp;
    ffp = fopen( "rpn.txt", "rb" );
    if( ffp != NULL ) {
        file_size = SizeOfFile(ffp);
        int variant = 1;
        do {
            PrintMenu();
            variant = GetVariant(2);
            switch(variant){
                case 1:{
                    printf("rpn: ");
                    char* filename;
                    FILE* fd;
                    char str[file_size-1];
                    filename = "rpn.txt";
                    fd = fopen(filename, "rt");
                    fgets(str,file_size,fd);
                    Schet(str);
                    printf("\n");
                    fclose(fd);
                } break;
                default:
                  break;
            }
        } while (variant != 2);
        Deletion();
        fclose(ffp);
        remove("rpn.txt");
    }
}
//________________________________________________________________________
void ErrorsInRPN(void) {
    printf("\nОшибка символом %c под номером %d\n",
           slovo,position);
    fprintf(file,"\nОшибка символом %c под номером %d\n",
            slovo,position);
    exit(1);
}
void PlusMinusImport(void){
    MultiplicationDivisionImportance();
    while(1){
        if(!slovo||slovo =='\n') break;
        if(slovo=='+'|| slovo=='-'){
            char buf = slovo;
            Sdvig(slovo);
            MultiplicationDivisionImportance();
            fprintf(file," %c ", buf);
        }else {
            break;
        }
    }
}
void MultiplicationDivisionImportance(void){
    PlusMinusImportAfterSmth();
    while(1){
        if(!slovo||slovo =='\n') break;
        if(slovo=='/'|| slovo=='*'){
            char buf = slovo;
            Sdvig(slovo);
            PlusMinusImportAfterSmth();
            fprintf(file," %c ", buf);
        }else {
            break;
        }
    }
}
void PlusMinusImportAfterSmth(void){
    if(slovo=='-'||slovo=='+'){
        char buf  = slovo;
        Sdvig(slovo);
        fprintf(file,"%c", buf);
        Upor();
    }else {
        Upor();
    }
}
void Upor(void){
    Perem();
    while (1) {
        break;
    }
}
void Perem(void){
    if (isdigit(slovo)) {
        Numberr();
    }else if((slovo)=='.'){
        NumberrAfterDot();
    }else if(isalpha(slovo)){
        Peremen();
    }
    else if(slovo =='('){
        Sdvig('(');
        PlusMinusImport();
        Sdvig(')');
    }else
        ErrorsInRPN();
}
void Peremen(void){
    if (slovo == '\0'|| slovo=='\n') return;
    fprintf(file," ");
    while((slovo >= 'a' && slovo <= 'z') || (slovo >= 'A' && slovo <= 'Z')){
        char buf  = slovo;
        Sdvig(slovo);
        fprintf(file,"%c", buf);
    }
    fprintf(file,"  ");
}
void Numberr(void){
    double number = 0;
    if (slovo == '\0'|| slovo=='\n') return;
    while (slovo >= '0' && slovo <= '9') {
        number = number * 10 + slovo - '0';
        Sdvig(slovo);
    }
    if (slovo == '.') {
        Sdvig(slovo);
        double weight = 1;
        while (slovo >= '0' && slovo <= '9') {
            weight /= 10;
            number = number + (slovo - '0') * weight;
            Sdvig(slovo);
        }
    }
    fprintf(file,"%.6F ", number);
}
void NumberrAfterDot(void){
    double number = 0;
    if (slovo == '\0'|| slovo=='\n') return;
    while (slovo >= '0' && slovo <= '9') {
        number = number * 10 + slovo - '0';
        Sdvig(slovo);
    }
    if (slovo == '.') {
        Sdvig(slovo);
        double weight = 1;
        while (slovo >= '0' && slovo <= '9') {
            weight /= 10;
            number = number + (slovo - '0') * weight;
            Sdvig(slovo);
        }
    }
    if (fabs(number) < FLT_EPSILON){
        --position;
        ErrorsInRPN();
    }
    fprintf(file,"%.6F ", number);
}
void Sdvig(int t) {
    if (slovo == t){
        slovo = getchar();
        position++;
    }
    else ErrorsInRPN();
}
//________________________________________________________________
void PrintMenu(void) {
    printf("1. Печать значения\n");
    printf("2. Выход\n");
}
int GetVariant(int c) {
    int v;
    char s[100];
    scanf("%s", s);
    while (sscanf(s, "%d", &v) != 1 || v < 1 || v > c) {
        printf("Incorrect input. Try again: ");
        scanf("%s", s);
    }
    return v;
}
//____________________________________________________________
int SizeOfFile(FILE *f)
{
    int p, size;
    p = (int)ftell(f);
    fseek(f, 0L, SEEK_END);
    size = (int)ftell(f);
    fseek(f, p, SEEK_SET);
    return(size);
}
size_t PotstanovkaPeremen(const char *src, char *dst, const char *old, const char *new){
    size_t l_old = strlen(old), l_new = strlen(new);
    size_t dst_length = 0;
    const char *src_current = src;
    do{
        const char *src_next = strstr(src_current, old);
        if (src_next == NULL){
            if (dst != NULL)
                strcpy(dst + dst_length, src_current);
            dst_length += strlen(src_current);
            break;
        }
        size_t n = src_next - src_current;
        if (dst != NULL)
            memcpy(dst + dst_length, src_current, n);
        dst_length += n;
        if (dst != NULL)
            memcpy(dst + dst_length, new, l_new);
        dst_length += l_new;
        src_current = src_next + l_old;
    }while (1);
    return dst_length;
}
void Schet(char *src){
    char *s, *ss;
    getchar();
    puts(src);
    char *istr;
    istr = strdup(src);
    for (int a=0; a<strlen(istr); ++a)
        if(isalpha(istr[a])!=0){
            vvod:
            printf("Введите переменную\n");
            s = Vvod(&len1);
            printf("Введите значение переменной\n");
            ss = Vvod(&len2);
            size_t dst_length = PotstanovkaPeremen(istr, NULL, s, ss);
            char dst[dst_length + 1];
            PotstanovkaPeremen(istr, dst, s, ss);
            free (istr);
            free (s);
            free (ss);
            istr = strdup(dst);
            puts(istr);
            for (int b=0; b<strlen(istr); ++b)
                if(isalpha(istr[b])!=0){
                    goto vvod;
                }
            ProverkaStroki(istr);
            printf("\n");
            VvodVStekIPechat();
            free (istr);
            return;
        }
    ProverkaStroki(istr);
    printf("\n");
    VvodVStekIPechat();
    free (istr);
    return;
}
char *Vvod(int *l){
    *l = 1;
    int capacity = 2;
    int size = 10;
    char *s = (char*) malloc(size * sizeof(char));
    if (s == NULL) {
        exit(1);
    }
    s[0] = ' ';
    char c = getchar();
    while (c != '\n') {
        s[(*l)++] = c;
        if (*l >= size){
            capacity *= 2;
            s = (char*) realloc(s, capacity * sizeof(char));
        }
        c = getchar();
    }
    s[*l] = ' ';
    s[(*l)+1] = '\0';
    return s;
}
//_____________________________________________________________
void ProverkaStroki(char *stroka){
    char *c;
    for (c = strtok(stroka, " \n\t");
         c != NULL;
         c = strtok(NULL, " \n\t")) {
        if (!Vsiatie(c)) {
            printf("     %*s^--ошибка... неразрешённая замена переменной или операция\n",
                   (int) (c - stroka), "");
            return;
        }
    }
}
void VvodVStekIPechat(void){
    double d;
    if(StekPop(&d)) {
        VvodVStekIPechat();
        printf("В формате LG:%20.14lg\n", d);
        printf("В формате  F:  %20.6F\n", d);
        StekPush(d);
    }
}
int StekPop(double *d){
    struct Stek *top;
    if(base == NULL){
        return 0;
    }
    top = base;
    *d = top->d;
    base = top->next;
    StekDeletion(top);
    return 1;
}
void StekPush(double d){
    struct Stek *Stek_p;
    Stek_p = Stek_new(d);
    Stek_p->next = base;
    base = Stek_p;
}
int UnaryPop(double *x){
    return(StekPop(x));
}
extern int BinaryPop(double *x, double *y){
    if(!StekPop(y)) {
        return 0;
    }
    if(!StekPop(x)) {
        StekPop(y);
        return 0;
    }
    return 1;
}
int Vsiatie(char *c){
    double d;
    if(VsiatieDouble(c, &d)) {
        StekPush(d);
        return 1;
    }
    else if(!strcmp(c, "+")) {
        return (Pluss());
    }
    else if(!strcmp(c, "-")) {
        return (Minuss());
    }
    else if(!strcmp(c, "*")) {
        return (Multiplication());
    }
    else if(!strcmp(c, "/")) {
        return (Division());
    }
    else
        return 0;
}
int VsiatieDouble(char *c, double *d){
    char *a;
    (*d) = strtod(c, &a);
    if (a != c || *a == '\0')
    {
        return 1;
    }
    return 0;
}
int Pluss(void){
    double x, y;
    if (!BinaryPop(&x, &y)) {
        return 0;
    }
    StekPush(x + y);
    return 1;
}
int Minuss(void){
    double x, y;
    if (!BinaryPop(&x, &y)) {
        return 0;
    }
    StekPush(x - y);
    return 1;
}
int Multiplication(void){
    double x, y;
    if (!BinaryPop(&x, &y)) {
        return 0;
    }
    StekPush(x * y);
    return 1;
}
int Division(void){
    double x, y;
    if (!BinaryPop(&x, &y)) {
        return 0;
    }
    if(fabs(y) < FLT_EPSILON) {
        StekPush(x);
        StekPush(y);
        return 0;
    }
    StekPush(x / y);
    return 1;
}
int Deletion(void){
    double d;
    while(1) {
        if(!StekPop(&d)) {
            break;
        }
    }
    return 1;
}
void StekDeletion(struct Stek *Stekk){
    free(Stekk);
}
