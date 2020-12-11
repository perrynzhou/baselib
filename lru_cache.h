/*************************************************************************
  > File Name: lru_cache.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 五 12/11 21:04:40 2020
 ************************************************************************/

#ifndef _LRU_CACHE_H
#define _LRU_CACHE_H
#include <stdint.h>
typedef uint32_t (*lru_hash_func)(void *key, uint32_t key_len);
typedef int (*lru_cmp_func)(void *key1, uint32_t key1_len, void *key2, uint32_t key2_len);
typedef int (*lru_val_func)(void *val);
typedef int (*lru_key_func)(void *key);
typedef struct lru_node_t
{
  void *key;
  void *val;
  uint32_t key_len;
  struct lru_node_t *prev;
  struct lru_node_t *next;
  void *link;
} lru_node;
typedef struct lru_list_t
{
  lru_node *head;
  lru_node *tail;
  lru_cmp_func cmp_func;
  lru_key_func key_func;
  lru_val_func val_func;
  uint32_t size;
} lru_list;
typedef struct lru_cache_t
{
  lru_list **tables;
  lru_list *list;
  uint32_t size;
  uint32_t cap;
  lru_hash_func hash_func;
} lru_cache;

lru_cache *lru_cache_create(uint32_t cap, lru_hash_func func, lru_cmp_func cmp_func, lru_key_func key_func, lru_val_func val_func);
int lru_cache_init(lru_cache *cache, uint32_t cap, lru_hash_func func, lru_cmp_func cmp_func, lru_key_func key_func, lru_val_func val_func);
void *lru_cache_put(lru_cache *cache, void *key, uint32_t key_len, void *val);
void *lru_cache_get(lru_cache *cache, void *key, uint32_t key_len);
void lru_cache_deinit(lru_cache *cache);
void lru_cache_destroy(lru_cache *cache);
#endif
