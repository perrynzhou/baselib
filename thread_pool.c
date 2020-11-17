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
#include <uuid/uuid.h>
#include <string.h>
#include <stdint.h>
#define get16bits(d) (*((const uint16_t *)(d)))
uint32_t super_fast_hash(const char *data, int32_t len)
{
    uint32_t hash = len, tmp;
    int32_t rem;

    if (len <= 1 || data == NULL)
        return 1;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (; len > 0; len--) {
        hash += get16bits(data);
        tmp = (get16bits(data + 2) << 11) ^ hash;
        hash = (hash << 16) ^ tmp;
        data += 2 * sizeof(uint16_t);
        hash += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3:
            hash += get16bits(data);
            hash ^= hash << 16;
            hash ^= data[sizeof(uint16_t)] << 18;
            hash += hash >> 11;
            break;
        case 2:
            hash += get16bits(data);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1:
            hash += *data;
            hash ^= hash << 10;
            hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}
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
    pool->sems =  calloc(thread_count, sizeof(sem_t));
    assert(pool->threads != NULL);
    pthread_mutex_init(&pool->mutex, NULL);
    int i;
    for(i=0;i<thread_count;i++) {
        sem_init(&pool->sems[i], 0, 0);

    }
    return 0;
  }
  return -1;
}
static void *thread_pool_run(void *arg)
{
   thread_pool *pool = (thread_pool *)arg;
  fprintf(stdout,"thread index = %d\n",pool->index);
  while (!pool->stop)
  {
    sem_wait(&pool->sems[pool->index]);
    if (pool->stop)
    {
      fprintf(stdout,"got stop %d\n",pool->sems[pool->index]);
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
    char key[36] = {'\0'};
    if (queue_len(pool->requests) >= pool->max_requests)
    {
      return -1;
    }
    pthread_mutex_lock(&pool->mutex);
    void **value = (void **)queue_push(pool->requests);
    *value = request;
    pthread_mutex_unlock(&pool->mutex);
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid, key);
    uint32_t index = super_fast_hash((char *)&key,strlen((char *)&key))%pool->thread_count;
    sem_post(&pool->sems[index]);
    return 0;
  }
  void thread_pool_start(thread_pool * pool)
  {
    int i = 0;
    for (; i < pool->thread_count; i++)
    {
        pthread_mutex_lock(&pool->mutex);
        pthread_create(&pool->threads[i], NULL,thread_pool_run,pool);
        pool->index=i;
        pthread_mutex_unlock(&pool->mutex);

    }
  }
  void thread_pool_stop(thread_pool * pool)
  {
    if(pool !=NULL) {
      int i=0;
      for(;i<pool->thread_count;i++) {
          pool->stop = true;
          sem_post(&pool->sems[i]);
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
     // sem_destroy(&pool->sem);
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