#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
int main()
{
	openlog("LogTest", LOG_PID|LOG_CONS, LOG_USER);
	setlogmask(LOG_UPTO(LOG_DEBUG));
	syslog(LOG_ERR, "ERR...\n");
	syslog(LOG_WARNING, "WARNING...\n");
	syslog(LOG_NOTICE, "NOTICE...\n");
	syslog(LOG_DEBUG, "Debug...\n");
	closelog();

	uid_t uid = getuid();
	uid_t euid = geteuid();
	printf("uid:%d euid:%d\n", uid, euid);
}
