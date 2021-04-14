#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h>
#include <locale.h>
int vflag = 0;
size_t rr = 0, ssize = 256; // ssize на ваш выбор
char ptr[4096]; //размер строк для вывода комментариев
void do_grep(int fd, const char *const filename,
             regex_t *const pattern, const int use_ansi);
void do_anti_grep(int fd, const char *const filename,
                  regex_t *const pattern, const int use_ansi);
//________________________________________________________________________
int main(int argc, char *argv[]){
    int ch;
    regex_t pattern;
    int     result, arg;
    int     use_ansi, fd = 0;
    while ((ch = getopt(argc, argv, "v")) != -1)
        if(ch == 'v'){
        vflag = 1;
            if(( vflag == 1) && (argc == 2)){
                rr = (write(2, "\n", strlen("\n")));
                sprintf(ptr,
                        "       %s -v PATTERN [ FILES ... ]\n",
                        argv[0]);
                rr = (write(2, ptr, strlen(ptr)));
                rr = (write(2, "\n", strlen("\n")));
                return EXIT_FAILURE;
            }
            for(int b = 0; b < argc; ++b){
                argv[b]= argv[b+1];
            }
            --argc;
    }
    if (argc < 2 || !strcmp(argv[1], "-h") ||
        !strcmp(argv[1],    "--help")){
        rr = (write(2, "\n", strlen("\n")));
        sprintf(ptr,"       %s PATTERN [ FILES ... ]\n",
                argv[0]);
        rr = (write(2, ptr, strlen(ptr)));
        rr = (write(2, "\n", strlen("\n")));
        return EXIT_FAILURE;
    }
    use_ansi = isatty(STDOUT_FILENO);
    result = regcomp(&pattern, argv[1], REG_NEWLINE);
    if (result){
        sprintf(ptr,"%s: Invalid pattern.\n", argv[1]);
        rr = (write(2, ptr, strlen(ptr)));
        regfree(&pattern);
        return EXIT_FAILURE;
    }
    if (argc > 2) {
        for (arg = 2; arg < argc; arg++) {
            fd = open(argv[arg], O_RDONLY, 0);
            if (fd == -1) {
                sprintf(ptr,
                        "%s: %s: No such file or directory\n",
                        argv[0], argv[arg]);
                rr = (write(2, ptr, strlen(ptr)));
                return EXIT_FAILURE;
            }
            else if(vflag != 1)
                    do_grep(fd, argv[arg], &pattern, use_ansi);
                else
                    do_anti_grep(fd, argv[arg], &pattern, use_ansi);
            if(close(fd) == -1) {
                sprintf(ptr,"%s: Read error.\n", argv[arg]);
                rr = (write(2, ptr, strlen(ptr)));
                return EXIT_FAILURE;
            }
        }
    }else{
        fd = fileno(stdin);
        if(vflag != 1)
            do_grep(fd, "(standard input)",
                    &pattern, use_ansi);
        else
            do_anti_grep(fd, "(standard input)",
                         &pattern, use_ansi);
        if (ferror(stdin)){
            sprintf(ptr,
                    "%s: Error reading from standard input.\n",
                    argv[0]);
            rr = (write(2, ptr, strlen(ptr)));
            regfree(&pattern);
            return EXIT_FAILURE;
        }
    }
    regfree(&pattern);
    return EXIT_SUCCESS;
}
//________________________________________________________________________
void do_grep(int fd, const char *const filename,
             regex_t *const pattern, const int use_ansi){
    char          p[]= {'\n', '\0'};
    size_t         i = 0, j = 0;
    size_t ssize2 = 1; // ssize2 на ваш выбор
    regmatch_t    match[2];
    struct stat sbuf;
    int wfd, n = 0;
    ssize_t nr;
    wfd = fileno(stdout);
    if (fstat(wfd, &sbuf) == -1)
        err(1, "stdout");
    char buf[ssize];
    char *bbuf = (char*)malloc(ssize2);
    memset(bbuf, '\0', ssize2);
    while ((nr = read(fd, buf, ssize)) != -1 && nr != 0){
        for(i = 0; i < nr; ++i){
            if(buf[i] == '\n'){
                ++n;
                if (j > 0 && bbuf[j - 1] == '\n')
                    bbuf[--j] = '\0';
                if (regexec(pattern, bbuf,
                            1, match, 0) == REG_NOMATCH)
                    goto nonono;
                sprintf(ptr, "%s: %d: ", filename, n);
                rr = write(1, ptr, strlen(ptr));
                if (use_ansi){
                    if (match[0].rm_so > 0)
                        rr = write(1, bbuf, match[0].rm_so);
                    if (match[0].rm_eo > match[0].rm_so) {
                        rr = write(1,
                                   "\033[1;31m",
                                   strlen("\033[1;31m"));
                        rr = write(1, bbuf +
                                   match[0].rm_so,
                                   match[0].rm_eo -
                                   match[0].rm_so);
                    }
                    rr = write(1, "\033[0m", strlen("\033[0m"));
                    if (match[0].rm_eo < j){
                        rr = write(1, bbuf +
                                   match[0].rm_eo, j -
                                   match[0].rm_eo);
                    }
                }else
                    rr = write(1,bbuf, j);
                    rr = write(1, p, 1);
 nonono:
                memset(bbuf, '\0', j);
                j=0;
            }
            else if(buf[i] != '\0'){
                bbuf[j] = buf[i];
                ++j;
                if (j >= ssize2){
                    ssize2 += ssize2;
                    bbuf = (char*) realloc(bbuf, ssize2 +
                                           sizeof(ssize2));
                    for(size_t y = j; y < ssize2; ++y)
                        bbuf[y] = '\0';
                }
            }
        }
    }
    free(bbuf);
}
//________________________________________________________________________
void do_anti_grep(int fd, const char *const filename,
                  regex_t *const pattern, const int use_ansi){
    char          p[]= {'\n', '\0'};
    size_t        i = 0, j = 0;
    size_t ssize2 = 1; // ssize2 на ваш выбор
    regmatch_t    match[2];
    struct stat sbuf;
    int wfd, n = 0;
    ssize_t nr;
    wfd = fileno(stdout);
    if (fstat(wfd, &sbuf) == -1)
        err(1, "stdout");
    char buf[ssize];
    char *bbuf = (char*)malloc(ssize2);
    memset(bbuf, '\0', ssize2);
    while ((nr = read(fd, buf, ssize)) != -1 && nr != 0){
        for(i = 0; i < nr; ++i){
            if(buf[i] == '\n'){
                ++n;
                if (j > 0 && bbuf[j - 1] == '\n')
                    bbuf[--j] = '\0';
                if (!(regexec(pattern, bbuf,
                              1, match, 0) == REG_NOMATCH))
                    goto nonono;
                sprintf(ptr, "%s: %d: ", filename, n);
                rr = write(1, ptr, strlen(ptr));
                rr = write(1,bbuf, j);
                rr = write(1, p, 1);
            nonono:
                memset(bbuf, '\0', j);
                j = 0;
            }
            else if(buf[i] != '\0'){
                bbuf[j] = buf[i];
                ++j;
                if (j >= ssize2){
                    ssize2 += ssize2;
                    bbuf = (char*) realloc(bbuf, ssize2 +
                                           sizeof(ssize2));
                    for(size_t y = j; y < ssize2; ++y)
                        bbuf[y] = '\0';
                }
            }
        }
    }
    free(bbuf);
}
