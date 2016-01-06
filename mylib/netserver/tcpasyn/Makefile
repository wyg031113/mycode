all:TcpAsynSelect control
CPP=g++
CC=gcc
CPPFLAGS=-g
TcpAsynSelect:TcpAsynSelect.cpp
	$(CPP) $(CPPFLAGS)  $< -o $@
control:control.cpp
	$(CPP) $(CPPFLAGS) $< -o $@
clean:
	rm TcpAsynSelect
