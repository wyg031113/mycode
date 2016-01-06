#ifndef __UTIL_H__
#define _UTIL_H___

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

#define CHECK(rv) {if((rv)<0){fprintf(stderr,"error! %s:(%d)-%s: %s\n", __FILE__, __LINE__, __func__, strerror(errno)); exit(rv);}}
#define CHECK2(rv, val) {rv = val; CHECK(rv);}

#endif /* __UTIL_H__*/
