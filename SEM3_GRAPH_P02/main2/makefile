COMPILER = gcc
DEBUG_COMPILER_FLAGS = -O0 -g -Wall -Werror -c
LINK_COMPILER_FLAGS = -o

 
graphs: graphs.o main2.o
	clang main2.o graphs.o -fsanitize=address -o graphs

graphs.o: graphs.c graphs.h
	clang -c -g -fsanitize=address graphs.c

main2.o: graphs.h main2.c
	clang -c -g -fsanitize=address main2.c