MAIN: main clean

main: prints.o cgi.o
	gcc -Wall main.c prints.o cgi.o -I. prints.h datatype.h -o main

prints.o:
	gcc -Wall prints.c -I. prints.h datatype.h -c

cgi.o:
	gcc -Wall cgi.c -I. cgi.h datatype.h -c

clean:
	rm -f *.o
