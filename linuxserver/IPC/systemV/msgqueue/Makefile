target=mqcreate mqunlink mq_sendmsg mq_recvmsg
all:$(target)
CFLAGS=-g -I../../
LIBS=-lrt
CC=gcc
mqcreate:mqcreate.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
mqunlink:mqunlink.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
mq_sendmsg:mq_sendmsg.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
mq_recvmsg:mq_recvmsg.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
run:
	./mqcreate
clean:
	rm -rf mqcreate *.o $(target)
