MAIN: main clean

main: prints.o cgi.o verificacao.o jogo.o
	gcc -Wall main.c prints.o cgi.o verificacao.o jogo.o -I. cgi.h jogo.h -o main

prints.o:
	gcc -Wall prints.c -I. prints.h datatype.h -c

cgi.o:
	gcc -Wall cgi.c -I. cgi.h datatype.h -c

verificacao.o:
	gcc -Wall verificacao.c -I. verificacao.h datatype.h -c

jogo.o:
	gcc -Wall jogo.c -I. prints.h datatype.h verificacao.h jogo.h -c

clean:
	rm -f *.o
