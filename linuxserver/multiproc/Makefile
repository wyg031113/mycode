all:forkexec sem1 sem2
CFLAGS=-ggdb3 -O0
CC=gcc
forkexec:forkexec.c
	$(CC) $(CFLAGS) $< -o $@
sem1:sem1.c
	$(CC) $(CFLAGS) $< -o $@
sem2:sem2.c
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf *.o forkexec
