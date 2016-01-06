target=timerevent
LIBDIR=../..
CFLAGS=-I$(LIBDIR)/include -ggdb3 -O0
LIBS=-L$(LIBDIR)/lib -ltcpserver -lmypoll -lmytimer
CC=gcc
CPP=g++
all:$(target)

$(target):timerevent.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
clean:
	rm -rf *.o *~ $(target)
