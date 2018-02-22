all : Exercicio1

Exercicio1 : Exercicio1.o
	gcc -o Exercicio1 Exercicio1.o

Exercicio1.o : Exercicio1.c
	gcc -c Exercicio1.c

clean :
	rm Exercicio1 *.o
