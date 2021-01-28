/*************************************************************************
  > File Name: daemon.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  8/16 10:34:22 2020
 ************************************************************************/

#include "daemon.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/syslog.h>
#include <sys/param.h>
#include <sys/stat.h>

int daemon(const char *name, daemon_func func, void *arg1, void *arg2, void *arg3)
{
  int pid;
  int i = 0;
  //ignore signal
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  //backgroud runing
  if (pid = fork())
  {
    exit(0);
  }
  else if (pid < 0)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  //leave process control
  setsid();
  //forbid reopen ternal
  if (pid = fork())
  {
    exit(0);
  }
  else if (pid < 0)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  //close fd
  for (; i < NOFILE; i++)
  {
    close(i);
  }
  //change workspace
  chdir("/tmp");
  //reset file master
  umask(0);

  //handle signal
  signal(SIGCHLD, SIG_IGN);

  //log output
  openlog(name, LOG_PID, 0);
  syslog(LOG_INFO, "%s runing,pid = %d\n", name, getpid());

  if (func != NULL)
  {
    func(arg1, arg2, arg3);
  }
  return 0;
}
#ifdef DAEMON_TEST
#include <string.h>
void *test(void *arg1, void *arg2, void *arg3)
{
  char *filename = (char *)arg1;
  //log output
  int fd = open(filename,O_CREAT|O_RDWR|O_TRUNC,0777);
  char buf[4096] = {'\0'};
  int i=0;
  while (1)
  {
    snprintf((char *)&buf,4096,"sleep %d\n",i);
    write(fd,(char *)&buf,strlen((char *)&buf));
    sleep(1);
    i++;
  }
  close(fd);
  return NULL;
}
int main(void)
{
  daemon("test_damon",test,"/var/log/test_damon.log",NULL,NULL);
  return 0;
}
#endif