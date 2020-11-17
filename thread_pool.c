/*************************************************************************
  > File Name: thread_pool.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 二 11/17 06:02:57 2020
 ************************************************************************/

#include "thread_pool.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

thread_pool *thread_pool_create(uint32_t max_requests, int thread_count, thread_func func)
{
  thread_pool *pool = calloc(1, sizeof(thread_pool));
  assert(pool != NULL);
  if (thread_pool_init(pool,max_requests, thread_count, func) != 0)
  {
    free(pool);
    pool = NULL;
  }
  return pool;
}
int thread_pool_init(thread_pool *pool, uint32_t max_requests, int thread_count, thread_func func)
{
  if (pool != NULL)
  {
    pool->func = func;
    pool->max_requests = max_requests;
    pool->requests = queue_create(sizeof(void **));
    pool->stop = false;
    pool->thread_count = thread_count;
    pool->threads = calloc(thread_count, sizeof(pthread_t));
    assert(pool->threads != NULL);
    pthread_mutex_init(&pool->mutex, NULL);
    sem_init(&pool->sem, 0, 0);
    return 0;
  }
  return -1;
}
static void *thread_pool_run(void *arg)
{
  thread_pool *pool = (thread_pool *)arg;
  while (!pool->stop)
  {
 
    sem_wait(&pool->sem);
    if (pool->stop)
    {
      break;
    }
    pthread_mutex_lock(&pool->mutex);
    if (queue_len(pool->requests) <= 0)
    {
      pthread_mutex_unlock(&pool->mutex);
      continue;
    }
    pthread_mutex_unlock(&pool->mutex);
    void **data = (void **)queue_pop(pool->requests);
    pool->func(*data);
  }
  return NULL;
}
  int thread_pool_append(thread_pool * pool, void *request)
  {
    if (queue_len(pool->requests) >= pool->max_requests)
    {
      return -1;
    }
    pthread_mutex_lock(&pool->mutex);
    void **value = (void **)queue_push(pool->requests);
    *value = request;
    pthread_mutex_unlock(&pool->mutex);
    sem_post(&pool->sem);
    return 0;
  }
  void thread_pool_start(thread_pool * pool)
  {
    int i = 0;
    for (; i < pool->thread_count; i++)
    {
      pthread_create(&pool->threads[i], NULL,thread_pool_run,pool);
    }
  }
  void thread_pool_stop(thread_pool * pool)
  {
    if(pool !=NULL) {
      pool->stop = true;
      int i=0;
      for(;i<pool->thread_count;i++) {
          sem_post(&pool->sem);
      }
    }
  }
  int thread_pool_deinit(thread_pool * pool)
  {
    if(pool!=NULL && pool->thread_count >0) 
    {
      int i=0;
      for(;i<pool->thread_count;i++)
      {
        pthread_join(pool->threads[i],NULL);
      }
      pthread_mutex_destroy(&pool->mutex);
      sem_destroy(&pool->sem);
      return 0;
    }
    return -1;
  }
  void thread_pool_destroy(thread_pool * pool)
  {
    if(thread_pool_deinit(pool)!=-1)
    {
      free(pool->threads);
      queue_destroy(pool->requests);
      free(pool);
    }
  }

  #ifdef THREAD_POOL_TEST
  int test_func(void *arg) {
    if(arg !=NULL) {
    int *v = (int *)arg;
    fprintf(stdout,"pthread id=%ld,handle value=%d\n",pthread_self(),*v);
    }
    return 0;
  }
  int main(int argc,char *argv[]) {
    thread_pool p1;
    thread_pool_init(&p1,10,2,test_func);
    thread_pool_start(&p1);
    int i=0,n=20;
    int *arrs = calloc(n,sizeof(int));
    for(;i<n/2;i++) {
      arrs[i] = rand()%1024-1;
      fprintf(stdout,"add ret=%d\n",thread_pool_append(&p1,&arrs[i]));
    }
    sleep(1);
    for(i=0;i<n/2;i++) {
      arrs[i] = rand()%1024-1;
      fprintf(stdout,"add ret=%d\n",thread_pool_append(&p1,&arrs[i]));
    }
    sleep(4);
    thread_pool_stop(&p1);
    thread_pool_deinit(&p1);
    return 0;
  }
  #endif