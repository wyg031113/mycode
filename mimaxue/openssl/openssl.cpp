#include <iostream>
#include <stdio.h>
using namespace std;

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

int main()
{
	cout<<"Hello world"<<endl;
	
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
	char dest[]="www.baidu.com:80";
	BIO *bio;
	bio = BIO_new_connect(dest);
	if(bio == NULL)
	{
		cerr<<"BIO_new_connect failed!"<<endl;
		exit(-1);
	}
	if(BIO_do_connect(bio) <= 0)
	{
		cerr<<"BIO_do_connect failed!"<<endl;
	}

	cout<<"connect success"<<endl;
	char data[]="GET / HTTP/1.0\r\n\r\n";
	int x = BIO_write(bio, data, strlen(data));
	if(x <= 0)
	{
		cout<<"write failed!"<<endl;
		exit(-1);
	}
	cout<<"write success"<<endl;
	char buf[512];
	while( (x = BIO_read(bio, buf, 512) > 0))
		cout<<buf<<endl;
	cout<<"read success"<<endl;

	BIO_free_all(bio);
	return 0;
}
