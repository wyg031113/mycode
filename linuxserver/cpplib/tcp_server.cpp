#include "tcp_server.h"
TcpServer::TcpServer(const char *ip /*="0.0.0.0"*/, unsigned short port)
{
	CHECK(listen_sock.fd = socket(PF_INET, SOCK_STREAM, 0));
	int ret = 0;
	bzero(&listen_sock.addr, sizeof(listen_sock));
	listen_sock.addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &listen_sock.addr.sin_addr);
	listen_sock.addr.sin_port = htons(port);
	int opt = 2;
	socklen_t len = sizeof(opt);
	setsockopt(listen_sock.fd, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	CHECK(ret = bind(listen_sock.fd, (struct sockaddr*)&listen_sock.addr, sizeof(listen_sock.addr)));
	CHECK(ret=listen(listen_sock.fd, 5));
}

Socket TcpServer::accept()
{
	Socket s;
	socklen_t len = sizeof(sockaddr);
	CHECK(::accept(listen_sock.fd, (sockaddr*)&s.addr, &len));
	return s;
}

int main()
{
	TcpServer ts;
	Socket s = ts.accept();
}
