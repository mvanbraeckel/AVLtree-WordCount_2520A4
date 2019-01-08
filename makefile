all: avltree

avltree: avltree.c
	gcc -Wall -pedantic -std=c99 avltree.c -o avltree

clean:
	rm -f *.o avltree all
