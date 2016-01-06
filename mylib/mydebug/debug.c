#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#define _DEBUG_IF_ 1
#define _DEBUG_ON_
#ifdef _DEBUG_ON_
#define DEBUGMSG(x) do {if (_DEBUG_IF_) { printf("%s(%d)-%s: ",__FILE__, __LINE__, __func__); debugmsg x;} }while (0)
#else
#define DEBUGMSG(x)
#endif
void debugmsg(const char *format, ...)
{
	va_list         debugargs;
	va_start(debugargs, format);
	vprintf(format, debugargs);
	va_end(debugargs);
}

int main()
{
	DEBUGMSG(("abcd is %d and %s\n", 123, "99999"));
	return 0;
}
