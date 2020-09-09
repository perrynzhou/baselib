/*************************************************************************
    > File Name: log.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Wednesday, September 09, 2020 AM08:34:22
 ************************************************************************/

#ifndef _LOG_H
#define _LOG_H
enum log_level
{
    LOG_TRACE_LEVEL,
    LOG_DEBUG_LEVEL,
    LOG_INFO_LEVEL,
    LOG_ERROR_LEVEL,
    LOG_FATAL_LEVEL,
    LOG_WARN_LEVEL
};
typedef struct log {
    char *name;  
    int  fd;
    char *buffer;
}log;

int log_init(char *filename);
static void log_write(const char *file, int line,int level, const char *fmt, ...);
void log_deinit(void);
#define log_fatal(...) log_write(__FILE__, __LINE__, LOG_FATAL_LEVEL, __VA_ARGS__)
#define log_debug(...) log_write(__FILE__, __LINE__, LOG_DEBUG_LEVEL, __VA_ARGS__)
#define log_info(...) log_write(__FILE__, __LINE__, LOG_INFO_LEVEL, __VA_ARGS__)
#define log_warn(...) log_write(__FILE__, __LINE__, LOG_WARN_LEVEL, __VA_ARGS__)
#define log_error(...) log_write(__FILE__, __LINE__, LOG_ERROR_LEVEL, __VA_ARGS__)
#define log_trace(...) log_write(__FILE__, __LINE__, LOG_TRACE_LEVEL, __VA_ARGS__)

#endif
