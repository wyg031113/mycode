#include <stdio.h>

#include "debug.h"

int main()
{
	OPENLOG;
	DEBUGMSG("Hello, %s %d\n", "2345", 454545);
	DEBUGLOG("Hello,syslog, %s %d\n", "2345", 454545);
	return 0;
}
