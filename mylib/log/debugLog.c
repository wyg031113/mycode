#include<syslog.h>
#include<stdio.h>
#include<stdarg.h>
#include"debugLog.h"
void opensyslog(const char *str)
{
#ifdef LOG_ON
	openlog(str, LOG_PID|LOG_CONS, LOG_USER);
#endif
}
void closesyslog()
{
#ifdef LOG_ON
	closelog();
#endif
}
void loginfo(const char *fmt, ...)
{
#ifdef LOG_ON
	char msg[512];
	va_list args;
	va_start(args, fmt);
	vsnprintf(msg,512, fmt, args);
	syslog(LOG_INFO, msg);
	va_end(args);
#endif
}

#ifdef DEBUG_ON
static FILE *dbgfile = NULL;
#endif
void debugMsg(const char *fmt, ...)
{
#ifdef DEBUG_ON
	va_list args;
	va_start(args, fmt);
	dbgfile = dbgfile==NULL?stderr:dbgfile;
	vfprintf(dbgfile, fmt, args);
	va_end(args);
#endif
}

