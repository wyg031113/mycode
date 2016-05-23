all:aio_echo
CC=gcc
CFLAGS=-ggdb3 -I../../include/ -L../../lib
LIBS=-ltcpserver -lrt
aio_echo:aio_echo.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

clean:
	rm *.o rm aio_echo
