COMPILER = gcc --coverage -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm


onlycalculation: onlycalculation.o
	$(COMPILER) onlycalculation.o -o onlycalculation

onlycalculation.o: onlycalculation.c
	$(COMPILER) -O0 -g -c onlycalculation.c

run:
	cat test|./script

