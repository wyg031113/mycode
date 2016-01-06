#include <inc.h>
int main()
{
	printf("MQ_OPEN_MAX = %ld, MQ_PRIO_MAX = %ld\n", sysconf(__SC_MQ_OPEN_MAX), sysconf(SC_MQ_PRIO_MAX));
	return0;
}
