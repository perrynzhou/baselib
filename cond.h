/*************************************************************************
  > File Name: cond.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 二 11/17 05:51:15 2020
 ************************************************************************/

#ifndef _COND_H
#define _COND_H
#include <pthread.h>
typedef struct cond_t {
  pthread_mutex_t pthd_mutex;
  pthread_cond_t pthd_cond;
}cond;

cond *cond_create();
int cond_init(cond *c);
bool cond_wait(cond *c);
bool cond_signal(cond *c);
int cond_deinit(cond *c);
void cond_destroy(cond *c);
#endif
