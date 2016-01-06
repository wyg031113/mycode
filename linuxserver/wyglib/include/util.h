#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <string.h>

#define SUCCESS 0
#define FAIL -1
#define CHECK(x) if((x)==-1){printf("%s(%s:%d)error:%s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno)); exit(-1);}
#define CHK_SUCC(x) if((x)!=SUCCESS){printf("%s(%s:%d) failed!\n", __FILE__, __FUNCTION__, __LINE__);exit(-1);}

#endif /*__UTIL_H__*/
