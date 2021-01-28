/*************************************************************************
    > File Name: mem_pool.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Saturday, August 01, 2020 AM11:22:32
 ************************************************************************/

#ifndef _MEM_POOL_H
#define _MEM_POOL_H
#include <stdint.h>
#include <stdio.h>
struct mem_pool_t;
typedef struct mem_pool_large_t
{
  struct mem_pool_large_t *next;
  void *alloc;
} mem_pool_large;
typedef struct mem_pool_data_t
{
  uint8_t *start;
  uint8_t *end;
  struct mem_pool_t *next;
} mem_pool_data;
typedef struct mem_pool_t
{
  mem_pool_data d;
  //if alloc memory overt max_alloc_size,use large to link big memory
  size_t max_alloc_size;
  struct mem_pool_t *current;
  struct mem_pool_large_t *large;
} mem_pool;

mem_pool *mem_pool_create(size_t max_alloc_size, size_t alloc_size);
void *mem_pool_alloc(mem_pool *pool, size_t size);
void mem_pool_free(mem_pool *pool);
#endif
