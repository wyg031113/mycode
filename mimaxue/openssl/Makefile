all:openssl openssl2
openssl:openssl.cpp
	g++ $< -lssl -o $@
openssl2:openssl2.cpp
	g++ $< -lssl -o $@
clean:
	rm -rf openssl openssl2
