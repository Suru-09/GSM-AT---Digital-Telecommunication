CC = gcc
CFLAGS = -o3

main : main.c
	$(CC) $(CFLAGS) -o main main.c parser.c

clean:
	rm -rf main