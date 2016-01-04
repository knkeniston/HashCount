words: main.o hash.o
	gcc main.o hash.o -o words

main.o: main.c hash.h
	gcc -std=c99 -c main.c

hash.o: hash.c hash.h
	gcc -std=c99 -c hash.c
