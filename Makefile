# all:
# 	gcc -Wall q.o -o Exercicio1
# q.o: gcc -Wall
all : Exercicio1

Exercicio1 : Exercicio1.o
	gcc -o Exercicio1 Exercicio1.o

Exercicio1.o : q.c
	gcc -c q.c

clean :
	rm q *.o
