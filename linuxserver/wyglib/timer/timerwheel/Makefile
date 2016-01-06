all:libmytimer main 
MYLIB=libmytimer.a
CFLAGS=-ggdb3 -O0
LIBOBJS=timer_wheel.o
libmytimer:$(LIBOBJS)
	ar -crs $(MYLIB) $(LIBOBJS)
main:main.c $(MYLIB)
	gcc -g main.c -L. -lmytimer -o main
install:$(MYLIB)
	cp $(MYLIB) ../../lib 
	cp timer_wheel.h ../../include
clean:
	rm -rf *.o main $(MYLIB)

