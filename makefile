CC = clang

all:	hello cat

hello:	hello.c
		$(CC) -Wall -std=c99 -o hello hello.c
cat:	cat.c
		$(CC) -Wall -std=c99 -o cat cat.c
clean:
		rm -rf *o hello cat	
