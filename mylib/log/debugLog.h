#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__
#ifdef __cplusplus 
extern "C"{
#endif
#define DEBUG_ON 
#define LOG_ON
void opensyslog(const char *str);
void closesyslog();
void loginfo(const char *fmt, ...);
void debugMsg(const char *fmt, ...);
#ifdef __cplusplus
}
#endif
#endif
