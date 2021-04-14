#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
size_t rr;
size_t bsize = 256;                   // размер для считывания
int nflag;
char *filename;
char ptr[4096];
void StrokiVisovCat(char *argv[]);
void StrokiKitCatFile(int rfd);       // cat -n file
void StrokiKitCat(int rfd);           // cat -n shell
void VisovCat(char *argv[]);
void KitCat(int);                     // cat shell and file
//________________________________________________________________________
int main(int argc, char **argv){
    int ch;
    setlocale(LC_ALL, "");
    while ((ch = getopt(argc, argv, "n")) != -1)
        if(ch == 'n')
        nflag = 1;
    argv += optind;
    if (nflag)
        StrokiVisovCat(argv);
    else
        VisovCat(argv);
    if (fclose(stdout)){
        rr = (write(2, "stdout", strlen("stdout")));
        exit(1);
    }
    return EXIT_SUCCESS;
}
//________________________________________________________________________
void StrokiVisovCat(char **argv){
    int fd;
    fd = fileno(stdin);
    filename = "stdin";
    do{
        if (*argv){
            if (!strcmp(*argv, "-"))
                fd = fileno(stdin);
            else if ((fd = open(*argv, O_RDONLY, 0)) == -1){
                sprintf(ptr,
                        "%s: No such file or directory\n",
                        *argv);
                rr = (write(2, ptr, strlen(ptr)));
                ++argv;
                continue;
            }
            filename = *argv++;
        }
        if (fd == fileno(stdin))
            StrokiKitCat(fd);
        else
            StrokiKitCatFile(fd);
        if (fd != fileno(stdin))
            if(close(fd) == -1){
                sprintf(ptr,
                        "%s: %s: Read error.\n",
                        argv[0], *argv);
                rr = (write(2, ptr, strlen(ptr)));
                exit(0);
            }
    }
    while (*argv);
}
//________________________________________________________________________
void StrokiKitCatFile(int rfd){
    int wfd, fff = 0, nn = 0, n = 0;
    ssize_t nr, r = 0;
    static char subbuf[1];
    struct stat sbuf;
    sprintf (ptr, "%s %d %s", "    ", n+1, " ");
    wfd = fileno(stdout);
    if (fstat(wfd, &sbuf) == -1){
        rr = (write(2, "stdout", strlen("stdout")));
        exit(0);
    }
    char buf[bsize];
    while ((nr = read(rfd, buf, bsize)) != -1 && nr != 0){
        if(nr != bsize)
            if(buf[nr-1] == '\n')
                buf[nr-1] = '\0';
        buf[nr] = '\0';
        r = nr;
        for(int i = 0; i < r; ++i){
            if(buf[i]  == '\0'){
            break;
            }
            if ((buf[0]  != '\0') && (i == 0) &&
                (fff == 0) && (buf[0]  != '\n') &&
                (fff == nn)){
                rr = write(wfd, ptr, strlen(ptr));
                nn++;
                n++;
            }
            if(buf[i]  == '\n' ){
                ++nn;
                n++;
                fff = -1;
                sprintf (ptr, "%s %d %s", "\n    ", n, " ");
                rr = write(wfd, ptr, strlen(ptr));
            }else{
                subbuf[0] = buf[i];
                rr = write(wfd, subbuf, 1);
            }
        }
        ++fff;
        memset (buf, '\0',  bsize);
    }
    subbuf[0] = '\n';
    rr = write(wfd, subbuf, 1);
    if (nr == -1){
        sprintf(ptr,"%s: Error reading from standard input.\n",
                filename);
        rr = (write(2, ptr, strlen(ptr)));
    }
}
//________________________________________________________________________
void StrokiKitCat(int rfd){
    int wfd, ff=0, n = 1;
    ssize_t nr;
    static char subbuf[1];
    struct stat sbuf;
    sprintf (ptr, "%s %d %s", "    ", n, "");
    wfd = fileno(stdout);
    if (fstat(wfd, &sbuf) == -1){
        rr = (write(2, "stdout", strlen("stdout")));
        exit(0);
    }
    char buf[bsize];
    while ((nr = read(rfd, buf, bsize)) != -1 && nr != 0){
        int i = 0;
        if ((buf[0]  != '\0') && (i == 0) && ff == 0)
            rr = write(wfd, ptr, strlen(ptr));
        for(; i < nr; ++i){
            if(buf[i]  == '\0')
                break;
            if(buf[i]  == '\n'){
                ++n;
                ff = -1;
                sprintf (ptr, "%s %d %s", "    ", n, "");
            }else{
                subbuf[0] = buf[i];
                rr = write(wfd, subbuf, 1);
            }
        }
        ++ff;
        if(ff == 0){
            subbuf[0] = '\n';
            rr = write(wfd, subbuf, 1);
        }
    }
    if (nr == -1){
        sprintf(ptr,
                "%s: Error reading from standard input.\n",
                filename);
        rr = (write(2, ptr, strlen(ptr)));
    }
}
//________________________________________________________________________
void VisovCat(char **argv){
    int fd;
    fd = fileno(stdin);
    filename = "stdin";
    do{
        if (*argv){
            if (!strcmp(*argv, "-"))
                fd = fileno(stdin);
            else if ((fd = open(*argv, O_RDONLY, 0)) == -1){
                sprintf(ptr,
                        "%s: No such file or directory\n",
                        *argv);
                rr = (write(2, ptr, strlen(ptr)));
                ++argv;
                continue;
            }
            filename = *argv++;
        }
        KitCat(fd);
        if (fd != fileno(stdin))
            if(close(fd) == -1) {
                sprintf(ptr,
                        "%s: %s: Read error.\n",
                        argv[0], *argv);
                rr = (write(2, ptr, strlen(ptr)));
                exit(1);
            }
    }
    while (*argv);
}
//________________________________________________________________________
void KitCat(int rfd){
    int wfd;
    ssize_t nr, nw, off;
    struct stat sbuf;
    char buf[bsize];
    wfd = fileno(stdout);
    if (fstat(wfd, &sbuf) == -1){
        rr = (write(2, "stdout", strlen("stdout")));
        exit(1);
    }
    while ((nr = read(rfd, buf, bsize)) != -1 && nr != 0)
        for (off = 0; nr; nr -= nw, off += nw)
    if ((nw = write(wfd, buf + off, (size_t)nr)) == 0 ||
        nw == -1){
        rr = (write(2, "stdout", strlen("stdout")));
        exit(1);
    }
    if (nr == -1){
        sprintf(ptr,
                "%s: Error reading from standard input.\n",
                filename);
        rr = (write(2, ptr, strlen(ptr)));
    }
}
