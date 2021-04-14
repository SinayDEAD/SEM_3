COMPILER = gcc --coverage -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm

 
graphs: main.o graphs.o
	$(COMPILER) main.o graphs.o -o graphs

main.o: main.c
	$(COMPILER) -O0 -g -c main.c

graphs.o: graphs.c graphs.h
	$(COMPILER) -O0 -g -c graphs.c
