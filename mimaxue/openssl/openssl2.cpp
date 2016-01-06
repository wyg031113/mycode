#include <iostream>
#include <stdio.h>
#include <stdarg.h>
using namespace std;

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#define CHECK0(p, format, ...) {if(p == 0){fprintf(stderr, "%s(%d)-%s\terror:%s\n", __FILE__, __LINE__, __func__,"(" #p")==NULL");fprintf(stderr, format, ##__VA_ARGS__); exit(-1);} }
#define CHECK1(p, format, ...) {if(p <= 0){fprintf(stderr, "%s(%d)-%s\terror:%s\n", __FILE__, __LINE__, __func__, #p"<=0");\
									  	  fprintf(stderr, format, ##__VA_ARGS__); exit(-1);} }
#define CHECK2(p, format, ...) {if(p < 0){fprintf(stderr, "%s(%d)-%s\terror:%s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, #p"<=0");\
								fprintf(stderr, format, ##__VA_ARGS__); exit(-1);} }

#define PORT 9999
#define LISTEN_BACKLOG 5
int main()
{
	cout<<"Hello world"<<endl;
	
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
	SSL_library_init();
	SSL_CTX *ctx;

	CHECK0( (ctx = SSL_CTX_new(SSLv2_server_method())), "SSL_CTX_new_failed\n");
	CHECK1( (SSL_CTX_use_certificate_file(ctx, "cacert.pem", SSL_FILETYPE_PEM)), "");
	CHECK1( (SSL_CTX_use_PrivateKey_file(ctx, "privkey.pem", SSL_FILETYPE_PEM)), "");
	CHECK0( SSL_CTX_check_private_key(ctx), "");

	int serfd, clifd;
	struct sockaddr_in seraddr;

	CHECK2( (serfd = socket(AF_INET, SOCK_STREAM, 0)),"");
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(PORT);
	seraddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&seraddr.sin_zero, 8);
	
	CHECK2( (bind(serfd, (struct sockaddr*)&seraddr, sizeof(struct sockaddr))), "");
	CHECK2( (listen(serfd, LISTEN_BACKLOG)), "");

	CHECK2(-1, "abcd%s", "hahaha");
	while(1)
	{
		SSL *ssl;
		CHECK2( (clifd = accept(serfd, NULL, NULL)), "");
		
		ssl = SSL_new(ctx);
		SSL_set_fd(ssl, clifd);
		CHECK2( (SSL_accept(ssl)), "");

		char buf[] = "Hello, this is ssl encrypt message\n";
		SSL_write(ssl, buf, strlen(buf));
		SSL_shutdown(ssl);
		SSL_free(ssl);
		close(clifd);
	}
	SSL_CTX_free(ctx);
	return 0;
}
