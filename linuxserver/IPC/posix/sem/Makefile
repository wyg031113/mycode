all:semcreate semunlink procon multiprocon anonprocon
CC=gcc
CFLAGS=-ggdb -O0 -I../../
LIBS=-lrt
semcreate:semcreate.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
semunlink:semunlink.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
procon:procon.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
multiprocon:multiprocon.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
anonprocon:anonprocon.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
clean:
	rm -rf *.o semcreate

