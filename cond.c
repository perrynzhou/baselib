/*************************************************************************
  > File Name: cond.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 二 11/17 05:51:24 2020
 ************************************************************************/

#include "cond.h"

cond *cond_create()
{
  cond *c = (cond *)calloc(1, sizeof(cond));
  assert(c != NULL);
  if (cond_init(c) != 0)
  {
    free(c);
    c = NULL;
  }
  return c;
}
int cond_init(cond *c)
{
  if (c != NULL)
  {
    if (pthread_mutex_init(&c->pthd_mutex, NULL) != 0)
    {
      return -1;
    }
    if (pthread_cond_init(&c->pthd_cond, NULL) != 0)
    {
      pthread_mutex_destroy(&c->pthd_mutex);
      return -1;
    }
  }
  return 0;
}
bool cond_wait(cond *c)
{
  int ret = 0;
  pthread_mutex_lock(&c->pthd_mutex);
  ret = pthread_cond_wait(&c->pthd_cond);
  pthread_mutex_unlock(&c->pthd_mutex);
  return ret == 0;
}
bool cond_signal(cond *c)
{
  return pthread_cond_broadcast(&c->pthd_cond);
}
int cond_deinit(cond *c)
{
  if (c != NULL)
  {
    pthread_mutex_destroy(&c->pthd_mutex);
    pthread_cond_destroy(&c->pthd_cond);
    return 0;
  }
  return -1;
}
void cond_destroy(cond *c)
{
  if (cond_deinit(c) != -1)
  {
    free(c);
    c = NULL;
  }
}