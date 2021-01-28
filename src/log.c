/*************************************************************************
    > File Name: log.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Wednesday, September 09, 2020 AM08:34:28
 ************************************************************************/

#include "log.h"
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define LOG_MAX_LEN (1024*1024)
static log g_log;
static const char *level_strings[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

static const char *level_colors[] = {
    "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"};

int log_init(char *filename)
{
    log *l = &g_log;
    if (filename == NULL)
    {
        l->fd = fileno(stdout);
        filename = "stdout";

    }
    else
    {
        l->fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (l->fd == -1)
        {
            return -1;
        }
    }
    l->name = strdup(filename);
    l->buffer = (char *)calloc(LOG_MAX_LEN,sizeof(char));
    log_info("log init file on %s",l->name);
    return 0;
}
static void log_format_write(const char *file, int line, int level, const char *fmt, va_list ap)
{
    log *l = &g_log;
    int len, size, errno_save;
    char *buf=l->buffer;
    ssize_t n;
    struct timeval tv;
    len = 0;
    size = LOG_MAX_LEN;
    gettimeofday(&tv, NULL);
    len += strftime(buf + len, size - len, "%Y-%m-%d %H:%M:%S.", localtime(&tv.tv_sec));
    len += snprintf(buf + len, size - len, "%03ld", tv.tv_usec / 1000);

    len += snprintf(buf + len, size - len, " %5s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ", level_colors[level], level_strings[level], file, line);

    len += vsnprintf(buf + len, size - len, fmt, ap);
    buf[len++] = '\n';
    n = write(l->fd, buf, len);
}
void log_write(const char *file, int line, int level, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_format_write(file, line, level, fmt, args);
    va_end(args);
    if(level==LOG_FATAL_LEVEL)
    {
        exit(0);
    }
}
void log_deinit(void)
{
    log *l = &g_log;
    if (l->fd != -1)
    {
        log_info("log deinit file on %s",l->name);
        close(l->fd);
        free(l->name);
    }
}
#ifdef LOG_TEST
int main(void)
{
    log_init(NULL);
    log_info("hi info");
    log_error("%s ni error", "ok");
    log_debug("%s hi ", "debug");
    log_warn("hi %s", "warn");
    log_trace("hi %s ,success %ld", "trace",100);
    int i=100;
    log_info("int address=%p",&i);

    log_deinit();
    return 0;
}
#endif