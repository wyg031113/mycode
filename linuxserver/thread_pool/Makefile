all:bin/main
CFLAGS=-ggdb3 -O0
LIBS= -lpthread
OBJS=bin/main.o bin/thread_pool.o
CPP=g++
bin/main:$(OBJS)
	$(CPP) $(CFLAGS) $(OBJS) $(LIBS) -o $@
%.c:%.o
	$(CPP) $(CFLAGS) -c -o bin/$@
clean:
	rm -rf $(OBJS) main

