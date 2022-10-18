CC = gcc
CFLAGS = -o3

main : main.c parser.o
	$(CC) $(CFLAGS) -o main main.c parser.o

parser.o : parser.c

clean:
	rm -rf main parser.o