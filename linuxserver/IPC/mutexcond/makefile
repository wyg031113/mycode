target=cancelthread cond procon recordlock
OBJS=$(foreach n, $(target),$(n).o)
SOURCES=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SOURCES))
all:$(SOURCES)
	@echo $(OBJS)
	@echo $(SOURCES)
	$(foreach t, $(target), gcc -I../ $(t).c -lpthread -o $(t);)
