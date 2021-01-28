/*************************************************************************
    > File Name: mem_pool.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Saturday, August 01, 2020 AM11:22:36
 ************************************************************************/
#include "mem_pool.h"
#include <stdio.h>
#define mem_pool_INIT_SIZE (1024)
#define mem_pool_MAX_SIZE (16384)
static void *mem_pool_alloc_block(mem_pool *pool, size_t size)
{
  uint8_t *m = NULL;
  size_t psize = (size_t)(pool->d.end - (uint8_t *)pool);
  mem_pool *p, *new;
  m = (uint8_t *)calloc(psize, sizeof(uint8_t));
  if (m == NULL)
  {
    return NULL;
  }
  new = (mem_pool *)m;
  new->d.end = m + psize;
  m += sizeof(mem_pool);
  new->d.start = m + size;
  for (p = pool->current; p != NULL; p = p->d.next)
  {
    pool->current = p->d.next;
  }
  pool->d.next = new;
  return m;
}
static void *mem_pool_alloc_small(mem_pool *pool, size_t size)
{
  uint8_t *m = NULL;
  mem_pool *p = pool->current;
  do
  {
    m = p->d.start;
    if ((size_t)(p->d.end - m) >= size)
    {
      p->d.start = m + size;
      return m;
    }
    p = p->d.next;
  } while (p);
  return mem_pool_alloc_block(pool, size);
}
static void *mem_pool_alloc_large(mem_pool *pool, size_t size)
{
  void *p = calloc(size, sizeof(uint8_t));
  mem_pool_large *large = NULL;
  uint32_t n = 0;
  for (large = pool->large; large != NULL; large = large->next)
  {
    if (large->alloc == NULL)
    {
      large->alloc = p;
      return p;
    }
    if (n++ > 3)
    {
      break;
    }
  }
  large = mem_pool_alloc_small(pool, sizeof(mem_pool_large));
  large->alloc = p;
  large->next = pool->large;
  pool->large = large;
  return p;
}
mem_pool *mem_pool_create(size_t max_alloc_size, size_t alloc_size)
{
  size_t size = (alloc_size < mem_pool_INIT_SIZE) ? (mem_pool_INIT_SIZE) : alloc_size;
  mem_pool *p = (mem_pool *)calloc(size, sizeof(char));
  assert(p != NULL);
  p->max_alloc_size = (max_alloc_size < mem_pool_MAX_SIZE) ? (mem_pool_MAX_SIZE) : max_alloc_size;
  p->d.start = (uint8_t *)p + sizeof(*p);
  p->d.end = (uint8_t *)p + size;
  p->d.next = NULL;
  p->current = p;
  return p;
}
void *mem_pool_alloc(mem_pool *pool, size_t size)
{
  if (size < pool->max_alloc_size)
  {
    return mem_pool_alloc_small(pool, size);
  }
  return mem_pool_alloc_large(pool, size);
}
void mem_pool_free(mem_pool *pool)
{
  mem_pool_large *large = NULL;
  for (large = pool->large; large != NULL; large = large->next)
  {
    free(large->alloc);
    free(large);
  }
  mem_pool *p = pool->d.next;
  for (p = pool->d.next; p != NULL; p = pool->d.next)
  {
    void *data = p;
    free(data);
  }
  free(pool);
  pool = NULL;
}