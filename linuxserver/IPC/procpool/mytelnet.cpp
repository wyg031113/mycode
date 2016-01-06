#include <inc.h>
#include "processpool.h"

class cgi_conn
{
public:
	cgi_conn(){}
	~cgi_conn(){}
	void init(int epollfd, int sockfd, const sockaddr_in& client_addr)
	{
		m_epollfd = epollfd;
		m_sockfd = sockfd;
		m_address = client_addr;
		memset(m_buf, 0, BUFFER_SIZE);
		m_read_idx = 0;
	}
	void process()
	{
		int idx = 0;
		int ret = -1;
		while(true)
		{
			idx = m_read_idx;
			ret = recv(m_sockfd, m_buf+idx, BUFFER_SIZE-1-idx, 0);
			if(ret<0)
			{
				if(errno != EAGAIN)
					removefd(m_epollfd, m_sockfd);
				break;
			}
			else if(ret == 0)
			{
				removefd(m_epollfd, m_sockfd);
			}
			else
			{
				m_read_idx += ret;
				printf("user content is:%s\n", m_buf);
				for(; idx < m_read_idx; ++idx)
				{
					if((idx>=1) && (m_buf[idx-1] == '\r') && (m_buf[idx]=='\n'))
						break;
				}
				if(idx == m_read_idx)
					continue;
				m_buf[idx-1] = 0;

				char *file_name = m_buf;
			/*	if(access(file_name, F_OK)==-1)
				{
					printf("access failed!\n");
					removefd(m_epollfd, m_sockfd);
					break;
				}
			*/

				ret = fork();
				if(ret == -1)
				{
					removefd(m_epollfd, m_sockfd);
					break;
				}
				else if(ret > 0)
				{
					//removefd(m_epollfd, m_sockfd);
					pause();
					//close(m_sockfd);
					char bx[BUFFER_SIZE];
					snprintf(bx, BUFFER_SIZE,"%s#", get_current_dir_name());
					send(m_sockfd, bx, strlen(bx), 0);
					m_read_idx = 0;
					break;
				}
				else
				{
					close(STDOUT_FILENO);
					dup(m_sockfd);
					int k;
					char *cmd[128];
					int  ci = 0;
					char prev = ' ';
					for(k = 0; k < m_read_idx; k++)
					{
						if(m_buf[k] == ' ')
						{
							prev = m_buf[k];
							m_buf[k] = 0;
						}
						else if(m_buf[k] !=' ' && prev == ' ')
						{
							prev = m_buf[k];
							//printf("find a arg!\n");
							if(ci>=128)
							{
								printf("too many argument!\n");
								break;
							}
							cmd[ci++] = m_buf+k;
						}
					}	
					cmd[ci++] = 0;
				//	printf("cmd:%s args:%s\n", cmd[0], cmd[1]);	
					if(strcmp(cmd[0], "exit")==0)
					{
						
						removefd(m_epollfd, m_sockfd);
						close(m_sockfd);

					}
					else
					{
						execvp(cmd[0], cmd);
						exit(0);
					}
				}
			}
		}
	}
private:
	static const int BUFFER_SIZE = 1024;
	static int m_epollfd;
	int m_sockfd;
	sockaddr_in m_address;
	char m_buf[BUFFER_SIZE];
	int m_read_idx;

};
int cgi_conn::m_epollfd = -1;
int main(int argc, char *argv[])
{
	if(argc <= 2)
		err_quit("usage %s ip port\n", basename(argv[0]));
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	int listenfd;
	CHECK(listenfd = socket(PF_INET, SOCK_STREAM, 0));
	int ret = 0;
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);
	int opt = 2;
	int len = sizeof(opt);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	CHECK(ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address)));
	CHECK(ret=listen(listenfd, 5));
	processpool<cgi_conn> *pool = processpool<cgi_conn>::create(listenfd);
	if(pool)
	{
		pool->run();
		delete pool;
	}
	close(listenfd);
	return 0;


}
