target=libtcpserver.a
all:$(target)

CFLAGS=-ggdb3 -O0
LIBOBJS=tcp_server.o
$(target):$(LIBOBJS)
	ar -crs $(target) $(LIBOBJS)
install:$(target)
	cp $(target) ../lib 
	cp tcp_server.h ../include
clean:
	rm -rf *.o $(target)

