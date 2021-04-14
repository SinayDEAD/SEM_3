#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
FILE *ff;
int gg = 0;
//--------------------------------------------------------------------------------------
void EnterBigString(FILE *f);  // ввод строки
int ParsingPoSimvolno(char *input); // парсинг сроки
//--------------------------------------------------------------------------------------
int Sizeoff(char **mas);
struct shell_struct *MakingTree(char **string);
struct shell_struct {
    int argc;
    char **argv;
    char *in; // ввода
    char *out; // вывода
    int kuda;
    int fon;
    struct shell_struct *dochern; // команды для запуска в дочернем shell
    struct shell_struct *pipe; // следующая команда после "|"
    struct shell_struct *after; // следующая после ";"
    int type;// ; = 1 или && = 1 или || = 2
};
struct structt {
    char **mas;
    int string1;
    int size;
    char *(*nexts)(struct structt *);
};
struct structt *Constructor(char **mas); // дерево
char *NextString(struct structt *iter); // следующий
struct shell_struct *Creation(struct structt *iter); // я родился
char Symbol(const char *string);  // проверка на особость
//--------------------------------------------------------------------------------------
int Pusk(struct shell_struct *unit);  //запускаем обработку
int CreatingChildren(struct shell_struct *unit);  //запускаем детей
struct shell_struct *Twister(struct shell_struct *unit);  // мотаем дальше леску
int Exucution(struct shell_struct *unit);   // ран батя ран
//--------------------------------------------------------------------------------------
void EmptyLine(char **mas);          //удаление строки
void EmptyTree(struct shell_struct *tree);  // удаление дерева
//--------------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    FILE *f1;
    if (argc < 2)
        f1 = stdin;
    else {
        if (!(f1 = fopen(argv[1], "r"))) {
            printf("No such file\n");
            return 0;
        }
    }
    EnterBigString(f1);
    return 0;
}
//--------------------------------------------------------------------------------------
void EnterBigString(FILE *f) {
    while (!feof(f)) {
        int c, cur_size = 0, max_size = 5;
        char *str;
        str = (char *) malloc(max_size * sizeof(char));
        gg = (int) write(1, ">>", 2);
        while (((c = fgetc(f)) != EOF) && (c != '\n')) {
            if (cur_size >= (max_size - 1)) {
                max_size *= 2;
                str = (char *) realloc(str, (max_size) * sizeof(char));
            }
            str[cur_size] = c;
            ++cur_size;
        }
        str[cur_size] = '\0';
        ParsingPoSimvolno(str);
    }
}

int ParsingPoSimvolno(char *input) {
    if (!strcmp(input, "")) {
        free(input);
        return 0;
    }
    int max_str = 5;
    int cur_str = 0;
    int i = 0;
    int chet = 0;
    char **p = (char **) malloc(max_str * sizeof(char *));
    int c;
    do {
        if (input[i] != '\0') {
            if (cur_str >= (max_str - 2)) {
                p = (char **) realloc(p, (max_str *= 2) * sizeof(char *));
            }
            p[cur_str] = (char *) malloc(5 * sizeof(char));
        }
        int cur_size = 0, max_size = 5;
        while ((c = input[i]) != '\0') {
            if (cur_size >= (max_size - 1)) {
                max_size *= 2;
                p[cur_str] = (char *) realloc(p[cur_str], (max_size) * sizeof(char));
            }
            if (((c == '<') || (c == ';') || (c == '(') || (c == ')'))) {
                char symb = c;
                if (cur_size > 0) {
                    p[cur_str][cur_size] = '\0';
                    cur_size = 0;
                    p[++cur_str] = (char *) malloc(4 * sizeof(char));
                }
                do {
                    p[cur_str][cur_size] = c;
                    ++cur_size;
                    c = input[++i];
                    ++chet;
                } while (c == symb && (chet < 1));
                --i;
                chet = 0;
                c = symb;
            } else if (((c == '|') || (c == '&') || (c == '>'))) {
                char symb = c;
                if (cur_size > 0) {
                    p[cur_str][cur_size] = '\0';
                    cur_size = 0;
                    p[++cur_str] = (char *) malloc(4 * sizeof(char));
                }
                do {
                    p[cur_str][cur_size] = c;
                    ++cur_size;
                    c = input[++i];
                    ++chet;
                } while (c == symb && (chet < 2));
                --i;
                chet = 0;
                c = symb;
            } else {
                switch (c) {
                    case ' ':
                        do { c = input[++i]; }
                        while (c == ' ');
                        --i;
                        c = ' ';
                        break;
                    case '"':
                        ++i;
                        while ((c = (input[i])) != '"') {
                            if (c == '\0') {
                                free(input);
                                for (int j = 0; j <= cur_str; j++) {
                                    free(p[j]);
                                }
                                free(p);
                                printf("error\n");
                                return 1;
                                break;
                            }
                            if (cur_size >= (max_size - 1)) {
                                max_size *= 2;
                                p[cur_str] = (char *) realloc(p[cur_str], (max_size) * sizeof(char));
                            }
                            p[cur_str][cur_size] = c;
                            cur_size++;
                            i++;
                        }
                        break;
                    default:
                        p[cur_str][cur_size] = c;
                        ++cur_size;
                        break;
                }
            }
            i++;
            if (((c == ' ') || (c == '|') || (c == '&') || (c == '>') || (c == '<') || (c == ';') || (c == '(') ||
                 (c == ')')) && (cur_size > 0)) {
                break;
            }
        }
        if (cur_size > 0) {
            p[cur_str][cur_size] = '\0';
            cur_str++;
        }
    } while (c != '\0');
    p[cur_str] = NULL;
    free(input);
    if (p == 0)
        return 0;
    if(cur_str == 0)
        return 0;
    int brace = 0;
    for (int ii = 0; p[ii] != NULL; ++ii) {
        if (strcmp(p[ii], "(") == 0)
            ++brace;
        else if (strcmp(p[ii], ")") == 0)
            if (--brace < 0){
                EmptyLine(p);
                return 1;
                }
    }
    if((strcmp("exit", *p)) == 0){
       EmptyLine(p);
       exit (0);
       }
    struct shell_struct *tree = MakingTree(p);
    Pusk(tree);
    EmptyTree(tree);
    EmptyLine(p);
    return 0;
}
struct shell_struct *MakingTree(char **string) {
    struct structt *iter = Constructor(string);
    struct shell_struct *root;
    root = Creation(iter);
    free(iter);
    return root;
}

struct structt *Constructor(char **mas) {
    struct structt *new = malloc(sizeof(struct structt));
    new->mas = mas;
    new->size = Sizeoff(mas);
    new->string1 = 0;
    new->nexts = NextString;
    return new;
}

char *NextString(struct structt *iter) {
    int n = iter->string1++;
    if (n >= iter->size) {
        return NULL;
    }
    return iter->mas[n];
}

struct shell_struct *Creation(struct structt *iter) {
    struct shell_struct *unit = malloc(sizeof(struct shell_struct));
    unit->argc = 0;
    unit->argv = malloc(sizeof(char *));
    unit->argv[0] = NULL;
    unit->in = NULL;
    unit->out = NULL;
    unit->kuda = 0;
    unit->fon = 0;
    unit->dochern = NULL;
    unit->pipe = NULL;
    unit->after = NULL;
    unit->type = 0;
    int f = 0;
    for (const char *lexeme = iter->nexts(iter); lexeme != NULL; lexeme = iter->nexts(iter)) {
        if (strcmp(lexeme, ">") == 0) {
            if (!Symbol(lexeme = iter->nexts(iter))) {
                EmptyTree(unit);
                return NULL;
            }
            unit->out = (char *) lexeme;
            unit->kuda = 0;
        } else if (strcmp(lexeme, ">>") == 0) {
            if (!Symbol(lexeme = iter->nexts(iter))) {
                EmptyTree(unit);
                return NULL;
            }
            unit->out = (char *) lexeme;
            unit->kuda = 1;
        } else if (strcmp(lexeme, "<") == 0) {
            if (!Symbol(lexeme = iter->nexts(iter))) {
                EmptyTree(unit);
                return NULL;
            }
            unit-> in= (char *) lexeme;
        } else if (strcmp(lexeme, ";") == 0) {
            if (!f) {
                EmptyTree(unit);
                return NULL;
            }
            unit->type = 0;
            unit->after = Creation(iter);
            break;

        } else if (strcmp(lexeme, "&&") == 0) {
            unit->type = 1;
            unit->after = Creation(iter);
            break;

        } else if (strcmp(lexeme, "||") == 0) {
            unit->type = 2;
            unit->after = Creation(iter);
            break;

        } else if (strcmp(lexeme, "&") == 0) {
            unit->fon = 1;
            unit->after = Creation(iter);
            break;

        } else if (strcmp(lexeme, "|") == 0) {
            unit->pipe = Creation(iter);
            break;

        } else if (strcmp(lexeme, "(") == 0) {
            if (unit->argc != 0) {
                EmptyTree(unit);
                return NULL;
            }
            unit->dochern = Creation(iter);
        } else if (strcmp(lexeme, ")") == 0) {
            if (!f) {
                EmptyTree(unit);
                return NULL;
            } else
                return unit;
        } else {
            char *string = malloc(sizeof(char) * (strlen(lexeme) + 1));
            strcpy(string, lexeme);
            int size = ++unit->argc;
            unit->argv = realloc(unit->argv, (size + 1) * sizeof(char *));
            unit->argv[size - 1] = string;
            unit->argv[size] = NULL;
            string = NULL;
        }
        f = 1;
    }
    if (!f) {
        EmptyTree(unit);
        return NULL;
    } else
        return unit;
}

char Symbol(const char *string) {
    for (int i = 0; i < strlen((string)); ++i) {
        switch (string[i]) {
            case '>':
            case '<':
            case '&':
            case '#':
            case '(':
            case ')':
            case '|':
            case ';':
                return 0;
            default:
                continue;
        }
    }
    return 1;
}

int Pusk(struct shell_struct *unit) {
    if (unit == NULL)
        return 0;
    int status = 0;
    if ((unit->argv[0] != NULL) && (strcmp(unit->argv[0], "cd") == 0)) {
        gg = chdir(unit->argv[1]);
        status = Pusk(unit->dochern);
        if (unit->after != NULL)
            if ((unit->type == 0) ||
                ((unit->type == 1) && (status == 0)) ||
                ((unit->type == 2) && (status != 0)))
                status = Pusk(unit->after);
        return status;
    }
    if (unit->pipe != NULL) {
        int in = dup(0);
        status = CreatingChildren(unit);
        dup2(in, 0);
        unit = Twister(unit);
        if (unit == NULL)
            return status;
        if (unit->after != NULL)
            if ((unit->type == 0) ||
                ((unit->type == 1) && (status == 0)) ||
                ((unit->type == 2) && (status != 0)))
                status = Pusk(unit->after);
        return status;
    }
    if (fork() == 0) {
        if (unit->out != NULL) {
            char buf[2];
            if (unit->kuda == 1)
                strcpy(buf, "a");
            else
                strcpy(buf, "w");
            ff = freopen(unit->out, buf, stdout);
        }

        if (unit->in)
            ff = freopen(unit->in, "r", stdin);
        if (unit->fon == 1) {
            signal(SIGINT, SIG_IGN);
            if (fork() == 0) {
                Exucution(unit);
                exit(0);
            } else
                kill(getpid(), SIGKILL);
        } else {
            status = Exucution(unit);
            if (status != 0){
                exit(1);
            }
            else{
                exit(0);
            }
        }
    } else {
        while(wait(&status) != -1)
        status = WEXITSTATUS(status);
        if (unit->after != NULL)
            if ((unit->type == 0) ||
                ((unit->type == 1) && (status == 0)) ||
                ((unit->type == 2) && (status != 0)))
                status = Pusk(unit->after);
        return status;
    }
    return 0;
}

int CreatingChildren(struct shell_struct *unit) {
    int pid;
    int status;
    int fd[2];
    while (unit != NULL) {
        gg = pipe(fd);
        pid = fork();
        if (pid == 0) {
            signal(SIGINT, SIG_DFL);
            if (unit->out != NULL) {
                char buf[2];
                if (unit->kuda == 1)
                    strcpy(buf, "a");
                else
                    strcpy(buf, "w");
                ff = freopen(unit->out, buf, stdout);
            }

            if (unit->in)
                ff = freopen(unit->in, "r", stdin);
            if (unit->pipe != NULL) {
                dup2(fd[1], 1);
            }
            close(fd[1]);
            close(fd[0]);
            if (Exucution(unit) == -1) {
                return -1;
            }
            exit(0);
        }
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        unit = unit->pipe;
    }
    while (wait(&status) != -1);
    return status;
}

struct shell_struct *Twister(struct shell_struct *unit) {
    if ((unit->pipe == NULL) && (unit->dochern == NULL))
        return unit;

    if (unit->pipe != NULL)
        return Twister(unit->pipe);

    if (unit->dochern != NULL)
        return Twister(unit->dochern);
    return 0;
}

int Exucution(struct shell_struct *unit) {
    int val = 0;
    if (unit->argv[0] == NULL) {
        val = Pusk(unit->dochern);
    } else {
        if (execvp(unit->argv[0], unit->argv) == -1) {
            return -1;
        }
    }
    return val;
}

int Sizeoff(char **mas) {
    int s = 0;
    while (*(mas++) != NULL)
        s++;
    return s;
}

void EmptyLine(char **mas) {
    if (mas == 0)
        return;
    int l = Sizeoff(mas);
    for (int i = 0; i <= l; i++)
        free((mas)[i]);
    free(mas);
}

void EmptyTree(struct shell_struct *tree) {
    if (tree == NULL)
        return;
    EmptyTree(tree->dochern);
    EmptyTree(tree->pipe);
    EmptyTree(tree->after);
    for(int i = 0; tree->argv[i] != NULL;++i)
    free(tree->argv[i]);
    free(tree->argv);
    free(tree);
}
