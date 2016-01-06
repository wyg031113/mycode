#include<syslog.h>
#include<stdio.h>
#include<stdarg.h>
#include"debugLog.h"
int main()
{
	openlog("wyglog", LOG_PID|LOG_CONS, LOG_USER);
	syslog(LOG_INFO, "This is a test of log %d",123);
	syslog(LOG_DEBUG, "Debug log %d",456);
	loginfo("Log info %d %s %d", 123, "abc", 456);
	closelog();
	debugMsg("this is debug %d %s\n", 10,"abc");
	return 0;
}
