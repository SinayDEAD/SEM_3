#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif
size_t rr;
//________________________________________________________________________
int main(int argc, char *argv[])
{
    ssize_t r;
    struct stat s;
    int flags, oldfile, newfile;
    char buf[BUF_SIZE];
    char ptr[4096];

    if (argc != 3 || strcmp(argv[1], "--help") == 0){
        sprintf(ptr,"%s old-file new-file\n",argv[0]);
        rr = (write(2, ptr, strlen(ptr)));
        exit(1);
    }
              
    oldfile = open(argv[1], O_RDONLY);
    if (oldfile == -1){
        sprintf(ptr,"opening file %s",argv[1]);
        rr = (write(2, ptr, strlen(ptr)));
        exit(1);
    }
    unlink(argv[2]);
    stat(argv[1], &s);
    flags = O_CREAT | O_WRONLY;
    newfile = open(argv[2], flags, 0777);
    if (newfile == -1){
        sprintf(ptr,"opening file %s\n",argv[2]);
        rr = (write(2, ptr, strlen(ptr)));
        exit(1);
    }

    while ((r = read(oldfile, buf, BUF_SIZE)) > 0)
        if (write(newfile, buf, r) != r){
            rr = (write(2,"write() error/partial write",
                        strlen("write() error/partial write ")));
            exit(1);
        }
    if (r == -1){
        rr = (write(2, "read", strlen("read")));
        exit(1);
    }
    if (close(oldfile) == -1){
        rr = (write(2, "close input", strlen("close input")));
        exit(1);
    }
    chmod(argv[2], s.st_mode);
    if (close(newfile) == -1){
        rr = (write(2, "close output", strlen("close output")));
        exit(1);
    }
    exit(EXIT_SUCCESS);
}
