MAIN: main

main: prints.o
	gcc -Wall main.c prints.o -I. prints.h datatype.h -o main

prints.o:
	gcc -Wall prints.c -I. prints.h datatype.h -c
