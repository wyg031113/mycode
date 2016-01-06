all:main
OBJS=thread.o main.o callback.o tcpserver.o
libs=-lpthread
main:$(OBJS)
	g++ -g $(OBJS) $(libs) -o $@
%.o:%.cpp
	g++ -g -c  $< -o $@
clean:
	rm -rf $(OBJS) main
