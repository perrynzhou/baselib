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
#include <string.h>
#include <stdint.h>
thread_pool *thread_pool_create(uint32_t max_requests, int thread_count, thread_func func)
{
  thread_pool *pool = calloc(1, sizeof(thread_pool));
  assert(pool != NULL);
  if (thread_pool_init(pool, max_requests, thread_count, func) != 0)
  {
    thread_pool_deinit(pool);
    free(pool);
    pool = NULL;
  }
  return pool;
}
static void *thread_pool_func(void *arg)
{
  thread_pool_ctx *ctx = (thread_pool_ctx *)arg;
  thread_pool *pool = ctx->pool;
  fprintf(stdout, " ctx index = %d\n", ctx->index);
  while (!ctx->stop)
  {
    sem_wait(&ctx->sem);
    if (ctx->stop)
    {
      fprintf(stdout, "got stop %d\n", ctx->index);
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
int thread_pool_init(thread_pool *pool, uint32_t max_requests, int thread_count, thread_func func)
{
  if (pool != NULL)
  {
    pool->func = func;
    pool->max_requests = max_requests;
    pool->requests = queue_create(sizeof(void **));
    pool->ctx = (thread_pool_ctx *)calloc(thread_count, sizeof(thread_pool_ctx));
    assert(pool->ctx != NULL);

    pool->thread_count = thread_count;
    pthread_mutex_init(&pool->mutex, NULL);
    pool->index = -1;
    pool->status = false;
    int i;
    for (i = 0; i < thread_count; i++)
    {
      pool->ctx[i].index = i;
      pool->ctx[i].pool = pool;
      pool->ctx[i].stop = false;
      sem_init(&pool->ctx[i].sem, 0, 0);
    }
    return 0;
  }
  return -1;
}

void thread_pool_run(thread_pool *pool)
{
  int i;
  for (i = 0; i < pool->thread_count; i++)
  {
    if (!pool->status)
    {
      pool->status = true;
    }
    thread_pool_ctx *ctx = &pool->ctx[i];
    pthread_create(&pool->ctx[i].tid, NULL, thread_pool_func, &pool->ctx[i]);
  }
}
int thread_pool_add_task(thread_pool *pool, void *request)
{
  if (queue_len(pool->requests) >= pool->max_requests)
  {
    return -1;
  }
  pthread_mutex_lock(&pool->mutex);
  void **value = (void **)queue_push(pool->requests);
  if (pool->index >= pool->thread_count)
  {
    pool->index = -1;
  }
  pool->index++;
  *value = request;
  pthread_mutex_unlock(&pool->mutex);
  sem_post(&pool->ctx[pool->index].sem);
  return 0;
}

void thread_pool_stop(thread_pool *pool)
{
  if (pool != NULL && pool->status)
  {
    int i = 0;
    for (; i < pool->thread_count; i++)
    {
      pool->ctx[i].stop = true;
      sem_post(&pool->ctx[i].sem);
    }
  }
}
int thread_pool_deinit(thread_pool *pool)
{
  if (pool != NULL && pool->thread_count > 0)
  {
    int i = 0;
    for (; i < pool->thread_count; i++)
    {
      if (pool->status)
      {
        pthread_join(pool->ctx[i].tid, NULL);
      }
      sem_destroy(&pool->ctx[i].sem);
    }
    pthread_mutex_destroy(&pool->mutex);
    queue_destroy(pool->requests);
    free(pool->ctx);
    return 0;
  }
  return -1;
}
void thread_pool_destroy(thread_pool *pool)
{
  if (thread_pool_deinit(pool) != -1)
  {
    free(pool);
  }
}

#ifdef THREAD_POOL_TEST
int test_func(void *arg)
{
  if (arg != NULL)
  {
    int *v = (int *)arg;
    fprintf(stdout, "pthread id=%ld,handle value=%d\n", pthread_self(), *v);
  }
  return 0;
}
int main(int argc, char *argv[])
{
  thread_pool p1;
  thread_pool_init(&p1, 10, 2, test_func);
  thread_pool_run(&p1);
  int i = 0, n = 20;
  int *arrs = calloc(n, sizeof(int));
  for (; i < n / 2; i++)
  {
    arrs[i] = rand() % 1024 - 1;
    fprintf(stdout, "add ret=%d\n", thread_pool_add_task(&p1, &arrs[i]));
  }
  sleep(1);
  for (i = 0; i < n / 2; i++)
  {
    arrs[i] = rand() % 1024 - 1;
    fprintf(stdout, "add ret=%d\n", thread_pool_add_task(&p1, &arrs[i]));
  }
  sleep(4);
  thread_pool_stop(&p1);
  thread_pool_deinit(&p1);
  return 0;
}
#endif