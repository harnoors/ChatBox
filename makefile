s: main.o list.o
	gcc -o s-talk final.o list.o -pthread

main.o :final.c list.h
	gcc -c final.c
clean:
	rm s-talk final.o
