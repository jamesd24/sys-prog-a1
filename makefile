CC = clang

all:	hello cat jShell

hello:	hello.c
		$(CC) -Wall -std=c99 -o hello hello.c
cat:	cat.c
		$(CC) -Wall -std=c99 -o cat cat.c
jShell:	jShell.c
		$(CC) -Wall -std=c99 -o jShell jShell.c -lreadline
clean:
		rm -rf *o hello cat jShell
