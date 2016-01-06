all:procon cond cancelthread recordlock
CFLAGS=-I../ -ggdb3 -O0 
LIBS=-lpthread
procon:procon.c
	gcc $(CFLAGS) $< $(LIBS) -o $@
cond:cond.c
	gcc $(CFLAGS) $< $(LIBS) -o $@
cancelthread:cancelthread.c
	gcc $(CFLAGS) $< $(LIBS) -o $@
recordlock:recordlock.c
	gcc $(CFLAGS) $< $(LIBS) -o $@
clean:
	rm -rf procon cond cancelthread
