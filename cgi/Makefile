MAIN: main clean

main: prints.o cgi.o verificacao.o
	gcc -Wall main.c prints.o cgi.o verificacao.o -I. prints.h datatype.h verificacao.h -o main

prints.o:
	gcc -Wall prints.c -I. prints.h datatype.h -c

cgi.o:
	gcc -Wall cgi.c -I. cgi.h datatype.h -c

verificacao.o:
	gcc -Wall verificacao.c -I. verificacao.h datatype.h -c

clean:
	rm -f *.o
