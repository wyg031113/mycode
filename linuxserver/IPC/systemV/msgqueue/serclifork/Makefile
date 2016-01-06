tar1=server
tar2=client
all:$(tar1) $(tar2)
CC=gcc
CFLAGS=-ggdb3 -O0 -I../../../
LIBS=
$(tar1):$(tar1).c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
$(tar2):$(tar2).c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
clean:
	rm -rf *.o $(tar1)

