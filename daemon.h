/*************************************************************************
  > File Name: cstl_daemon.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 10:34:16 2020
 ************************************************************************/

#ifndef _CSTL_DAEMON_H
#define _CSTL_DAEMON_H
typedef void *daemon_func(void *,void *,void *);
int cstl_daemon(const char *name,daemon_func func,void *arg1,void *arg2,void *arg3);
#endif
