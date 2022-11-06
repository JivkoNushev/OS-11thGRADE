FILES = main.c wc.c head.c tail.c utils.c

all:
	gcc -Wall wc.c utils.c -o wc.o
	gcc -Wall head.c utils.c -o head.o
	gcc -Wall tail.c utils.c -o tail.o

wc:
	gcc -Wall wc.c utils.c -o wc.o

head:
	gcc -Wall head.c utils.c -o head.o

tail:
	gcc -Wall tail.c utils.c -o tail.o

rm:
	rm *.o *.out