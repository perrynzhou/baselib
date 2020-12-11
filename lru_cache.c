/*************************************************************************
  > File Name: lru_cache.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 五 12/11 21:04:46 2020
 ************************************************************************/

#include "lru_cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#if (defined(__GNUC__) && defined(__i386__))
#define get16bits(d) (*((const uint16_t *)(d)))
#endif

#if !defined(get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8) + (uint32_t)(((const uint8_t *)(d))[0]))
#endif
uint32_t hash_super_fast(const char *pdata, int32_t len)
{
  const char *data = (const char *)pdata;
  uint32_t hash = len, tmp;
  int32_t rem;

  if (len <= 1 || data == NULL)
    return 1;

  rem = len & 3;
  len >>= 2;

  /* Main loop */
  for (; len > 0; len--)
  {
    hash += get16bits(data);
    tmp = (get16bits(data + 2) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    data += 2 * sizeof(uint16_t);
    hash += hash >> 11;
  }

  /* Handle end cases */
  switch (rem)
  {
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

inline static lru_node *lru_node_create(void *key, uint32_t key_len, void *val)
{
  lru_node *node = calloc(1, sizeof(lru_node));
  assert(node != NULL);
  node->key = key;
  node->key_len = key_len;
  node->val = val;
  node->prev = node->next = NULL;
  node->link = NULL;
  return node;
}
inline static void lru_node_destroy(lru_node *node)
{
  if (node != NULL)
  {
    free(node);
    node = NULL;
  }
}
lru_list *lru_list_create(lru_cmp_func cmp_func, lru_key_func key_func, lru_val_func val_func)
{
  lru_list *list = (lru_list *)calloc(1, sizeof(lru_list));
  assert(list != NULL);
  list->cmp_func = cmp_func;
  list->key_func = key_func;
  list->val_func = val_func;
  return list;
}
lru_node *lru_list_find(lru_list *list, void *key, uint32_t key_len)
{
  if (list == NULL || list->size == 0)
  {
    return NULL;
  }
  lru_node *current = list->head;
  while (current != NULL)
  {
    lru_node *next = current->next;
    if (list->cmp_func(current->key, current->key_len, key, key_len) == 0)
    {
      return current;
    }
    current = next;
  }
  return NULL;
}
void lru_list_destroy(lru_list *list)
{
  lru_node *current = list->head;
  while (current != NULL)
  {
    lru_node *next = current->next;
    list->key_func(current->key);
    list->val_func(current->val);
    lru_node_destroy(current);
    current = next;
  }
  free(list);
  list = NULL;
}
static int lru_list_push(lru_list *list, lru_node *node)
{
  if (list == NULL || node == NULL)
  {
    return -1;
  }
  if (list->size == 0)
  {
    list->head = list->tail = node;
  }
  else
  {
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
  }
  __sync_fetch_and_add(&list->size, 1);
  return 0;
}
static int lru_list_pop(lru_list *list, lru_node *node)
{
  if (list->size > 0)
  {
    if (node->prev != NULL)
    {
      node->prev->next = node->next;
    }
    else
    {
      list->head = node->next;
    }
    if (node->next != NULL)
    {
      node->next->prev = node->prev;
    }
    else
    {
      list->tail = node->prev;
    }
    __sync_fetch_and_sub(&list->size, 1);
    node->prev = node->next = NULL;
    return 0;
  }
  return -1;
}
lru_cache *lru_cache_create(uint32_t cap, lru_hash_func func, lru_cmp_func cmp_func, lru_key_func key_func, lru_val_func val_func)
{
  lru_cache *cache = calloc(1, sizeof(lru_cache));
  assert(cache != NULL);
  if (lru_cache_init(cache, cap, cmp_func, key_func, val_func) != 0)
  {
    free(cache->tables);
    free(cache);
    cache = NULL;
  }
  return cache;
}

int lru_cache_init(lru_cache *cache, uint32_t cap, lru_hash_func hash_func, lru_cmp_func cmp_func, lru_key_func key_func, lru_val_func val_func)
{
  if (cache == NULL || cap <= 0 || cmp_func == NULL || key_func == NULL || val_func != NULL)
  {
    return -1;
  }
  cache->tables = calloc(cap, sizeof(lru_node **));
  for (int i = 0; i < cap; i++)
  {
    cache->tables[i] = lru_list_create(cmp_func, key_func, val_func);
  }
  assert(cache->tables != NULL);
  cache->size = 0;
  cache->cap = cap;
  cache->list->head = cache->list->tail = NULL;
  cache->hash_func = hash_func;
  if (cache->hash_func == NULL)
  {
    cache->hash_func = (lru_hash_func)hash_super_fast;
  }
  return 0;
}
static int lru_cache_expire(lru_cache *cache)
{

  if (cache == NULL || cache->list == NULL || cache->list->size == 0)
  {
    return -1;
  }
  lru_list *expire_list = cache->list;
  lru_node *expire_node = expire_list->tail;
  lru_list *index_list = (lru_list *)expire_node->link;
  lru_node *remove_node = lru_list_find(index_list, expire_node->key, expire_node->key_len);
  if (remove_node != NULL)
  {
    lru_list_pop(expire_list, remove_node);
    index_list->key_func(remove_node->key);
    index_list->val_func(remove_node->val);
    lru_node_destroy(remove_node);
  }
  if (expire_node != NULL)
  {
    lru_list_pop(expire_list, expire_node);
    expire_list->key_func(expire_node->key);
    expire_list->val_func(expire_node->val);
    lru_node_destroy(expire_node);
  }
  return 0;
}
void *lru_cache_put(lru_cache *cache, void *key, uint32_t key_len, void *val)
{
  uint32_t index = cache->hash_func(key, key_len);
  lru_list *index_list = cache->tables[index];
  lru_node *cache_node = lru_list_find(index_list, key, key_len);
  if (cache_node != NULL)
  {
    if (index_list->val_func != NULL)
    {
      index_list->val_func(cache_node->val);
      cache_node->val = val;
    }
    lru_node *link_node = (lru_node *)cache_node->link;
    lru_list_pop(cache->list, link_node);
    lru_list_push(cache->list, link_node);
    return cache_node->val;
  }
  if (cache->size >= cache->cap)
  {
    lru_cache_expire(cache);
  }
  lru_node *new_node = lru_node_create(key, key_len, val);
  lru_node *link_node = lru_node_create(key, key_len, val);
  new_node->link = link_node;
  link_node->link = index_list;
  lru_list_push(index_list, new_node);
  lru_list_push(cache->list, link_node);
  return link_node->val;
}
void *lru_cache_get(lru_cache *cache, void *key, uint32_t key_len)
{
  uint32_t index = cache->hash_func(key, key_len);
  lru_list *index_list = cache->tables[index];
  lru_node *cache_node = lru_list_find(index_list, key, key_len);
  if (cache_node != NULL)
  {

    lru_node *link = (lru_node *)cache_node->link;
    lru_list_pop(cache->list, link);
    lru_list_push(cache->list, link);
    return cache_node->val;
  }
  return NULL;
}
void lru_cache_deinit(lru_cache *cache)
{
  if (cache != NULL)
  {
    for (int i = 0; i < cache->cap; i++)
    {
      lru_list *list = cache->tables[i];
      if (list != NULL)
      {
        lru_list_destroy(list);
      }
      cache->tables[i] = NULL;
    }
    lru_list_destroy(cache->list);
    free(cache->tables);
  }
}
void lru_cache_destroy(lru_cache *cache)
{
  lru_cache_deinit(cache);
  if (cache != NULL)
  {
    free(cache);
  }
}
