all:testlog lib
OBJS=debugLog.o testlog.o
CPP=g++
CC=gcc
testlog:$(OBJS)
	$(CPP) $(OBJS) -o $@
debugLog.o: debugLog.c
	$(CC) -c $< -o $@

testlog.o: testlog.cpp
	$(CC) -c $< -o $@
lib:debugLog.o	
	ar -rcs libdebugLog.a debugLog.o
install:
	cp libdebugLog.a /usr/local/lib
	cp debugLog.h /usr/local/include
uninstall:
	rm /usr/local/lib/libdebugLog.a /usr/local/include/debugLog.h
clean:
	rm $(OBJS) testlog libdebugLog.a
