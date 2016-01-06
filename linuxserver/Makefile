target=netapi tcp_server tcp_client
all:$(target)
netapi:netapi.c
	gcc -g $< -o $@
tcp_server:tcp_server.c
	gcc -g $< -o $@
tcp_client:tcp_client.cpp
	g++ -g $< -o $@
clean:
	rm -rf $(target)
