all:testServer testServer2 testClient2 udpServer udpClient
CC=gcc
CPP=g++
CFLAGS=-g
CPPFLAGS=-g
testServer:testServer.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<
testServer2:testServer2.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<
testClient2:testClient2.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<
udpServer:udpServer.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<
udpClient:udpClient.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<
clean:
	rm testServer testServer2 testClient2 udpServer
