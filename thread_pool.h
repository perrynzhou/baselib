/*************************************************************************
  > File Name: thread_pool.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 二 11/17 06:02:52 2020
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include <pthread.h>
#include <stdint.h>
#include <semaphore.h>
#include "queue.h"
typedef int (*thread_func)(void *);
typedef struct thread_pool_ctx_t {
  pthread_t tid;
  sem_t  sem;
  int index;
  bool stop;
  void *pool;
}thread_pool_ctx;
typedef struct thread_pool_t
{
  thread_pool_ctx *ctx;
  pthread_mutex_t mutex;
  int thread_count;
  uint32_t max_requests;
  queue *requests;
  thread_func func;
  int index;
  bool status;
} thread_pool;

thread_pool *thread_pool_create(uint32_t max_requests, int thread_count, thread_func func);
int thread_pool_init(thread_pool *pool, uint32_t max_requests, int thread_count, thread_func func);
void thread_pool_run(thread_pool *pool);
int thread_pool_add_task(thread_pool *pool, void *request);
void thread_pool_stop(thread_pool *pool);
int thread_pool_deinit(thread_pool *pool);
void thread_pool_destroy(thread_pool *pool);
#endif
