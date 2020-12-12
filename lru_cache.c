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
static const uint32_t crc32tab[256] = {
    0x00000000,
    0x77073096,
    0xee0e612c,
    0x990951ba,
    0x076dc419,
    0x706af48f,
    0xe963a535,
    0x9e6495a3,
    0x0edb8832,
    0x79dcb8a4,
    0xe0d5e91e,
    0x97d2d988,
    0x09b64c2b,
    0x7eb17cbd,
    0xe7b82d07,
    0x90bf1d91,
    0x1db71064,
    0x6ab020f2,
    0xf3b97148,
    0x84be41de,
    0x1adad47d,
    0x6ddde4eb,
    0xf4d4b551,
    0x83d385c7,
    0x136c9856,
    0x646ba8c0,
    0xfd62f97a,
    0x8a65c9ec,
    0x14015c4f,
    0x63066cd9,
    0xfa0f3d63,
    0x8d080df5,
    0x3b6e20c8,
    0x4c69105e,
    0xd56041e4,
    0xa2677172,
    0x3c03e4d1,
    0x4b04d447,
    0xd20d85fd,
    0xa50ab56b,
    0x35b5a8fa,
    0x42b2986c,
    0xdbbbc9d6,
    0xacbcf940,
    0x32d86ce3,
    0x45df5c75,
    0xdcd60dcf,
    0xabd13d59,
    0x26d930ac,
    0x51de003a,
    0xc8d75180,
    0xbfd06116,
    0x21b4f4b5,
    0x56b3c423,
    0xcfba9599,
    0xb8bda50f,
    0x2802b89e,
    0x5f058808,
    0xc60cd9b2,
    0xb10be924,
    0x2f6f7c87,
    0x58684c11,
    0xc1611dab,
    0xb6662d3d,
    0x76dc4190,
    0x01db7106,
    0x98d220bc,
    0xefd5102a,
    0x71b18589,
    0x06b6b51f,
    0x9fbfe4a5,
    0xe8b8d433,
    0x7807c9a2,
    0x0f00f934,
    0x9609a88e,
    0xe10e9818,
    0x7f6a0dbb,
    0x086d3d2d,
    0x91646c97,
    0xe6635c01,
    0x6b6b51f4,
    0x1c6c6162,
    0x856530d8,
    0xf262004e,
    0x6c0695ed,
    0x1b01a57b,
    0x8208f4c1,
    0xf50fc457,
    0x65b0d9c6,
    0x12b7e950,
    0x8bbeb8ea,
    0xfcb9887c,
    0x62dd1ddf,
    0x15da2d49,
    0x8cd37cf3,
    0xfbd44c65,
    0x4db26158,
    0x3ab551ce,
    0xa3bc0074,
    0xd4bb30e2,
    0x4adfa541,
    0x3dd895d7,
    0xa4d1c46d,
    0xd3d6f4fb,
    0x4369e96a,
    0x346ed9fc,
    0xad678846,
    0xda60b8d0,
    0x44042d73,
    0x33031de5,
    0xaa0a4c5f,
    0xdd0d7cc9,
    0x5005713c,
    0x270241aa,
    0xbe0b1010,
    0xc90c2086,
    0x5768b525,
    0x206f85b3,
    0xb966d409,
    0xce61e49f,
    0x5edef90e,
    0x29d9c998,
    0xb0d09822,
    0xc7d7a8b4,
    0x59b33d17,
    0x2eb40d81,
    0xb7bd5c3b,
    0xc0ba6cad,
    0xedb88320,
    0x9abfb3b6,
    0x03b6e20c,
    0x74b1d29a,
    0xead54739,
    0x9dd277af,
    0x04db2615,
    0x73dc1683,
    0xe3630b12,
    0x94643b84,
    0x0d6d6a3e,
    0x7a6a5aa8,
    0xe40ecf0b,
    0x9309ff9d,
    0x0a00ae27,
    0x7d079eb1,
    0xf00f9344,
    0x8708a3d2,
    0x1e01f268,
    0x6906c2fe,
    0xf762575d,
    0x806567cb,
    0x196c3671,
    0x6e6b06e7,
    0xfed41b76,
    0x89d32be0,
    0x10da7a5a,
    0x67dd4acc,
    0xf9b9df6f,
    0x8ebeeff9,
    0x17b7be43,
    0x60b08ed5,
    0xd6d6a3e8,
    0xa1d1937e,
    0x38d8c2c4,
    0x4fdff252,
    0xd1bb67f1,
    0xa6bc5767,
    0x3fb506dd,
    0x48b2364b,
    0xd80d2bda,
    0xaf0a1b4c,
    0x36034af6,
    0x41047a60,
    0xdf60efc3,
    0xa867df55,
    0x316e8eef,
    0x4669be79,
    0xcb61b38c,
    0xbc66831a,
    0x256fd2a0,
    0x5268e236,
    0xcc0c7795,
    0xbb0b4703,
    0x220216b9,
    0x5505262f,
    0xc5ba3bbe,
    0xb2bd0b28,
    0x2bb45a92,
    0x5cb36a04,
    0xc2d7ffa7,
    0xb5d0cf31,
    0x2cd99e8b,
    0x5bdeae1d,
    0x9b64c2b0,
    0xec63f226,
    0x756aa39c,
    0x026d930a,
    0x9c0906a9,
    0xeb0e363f,
    0x72076785,
    0x05005713,
    0x95bf4a82,
    0xe2b87a14,
    0x7bb12bae,
    0x0cb61b38,
    0x92d28e9b,
    0xe5d5be0d,
    0x7cdcefb7,
    0x0bdbdf21,
    0x86d3d2d4,
    0xf1d4e242,
    0x68ddb3f8,
    0x1fda836e,
    0x81be16cd,
    0xf6b9265b,
    0x6fb077e1,
    0x18b74777,
    0x88085ae6,
    0xff0f6a70,
    0x66063bca,
    0x11010b5c,
    0x8f659eff,
    0xf862ae69,
    0x616bffd3,
    0x166ccf45,
    0xa00ae278,
    0xd70dd2ee,
    0x4e048354,
    0x3903b3c2,
    0xa7672661,
    0xd06016f7,
    0x4969474d,
    0x3e6e77db,
    0xaed16a4a,
    0xd9d65adc,
    0x40df0b66,
    0x37d83bf0,
    0xa9bcae53,
    0xdebb9ec5,
    0x47b2cf7f,
    0x30b5ffe9,
    0xbdbdf21c,
    0xcabac28a,
    0x53b39330,
    0x24b4a3a6,
    0xbad03605,
    0xcdd70693,
    0x54de5729,
    0x23d967bf,
    0xb3667a2e,
    0xc4614ab8,
    0x5d681b02,
    0x2a6f2b94,
    0xb40bbe37,
    0xc30c8ea1,
    0x5a05df1b,
    0x2d02ef8d,
};
uint32_t hash_crc32(const char *key, size_t key_length)
{
  uint64_t x;
  uint32_t crc = UINT32_MAX;

  for (x = 0; x < key_length; x++)
  {
    crc = (crc >> 8) ^ crc32tab[(crc ^ (uint64_t)key[x]) & 0xff];
  }

  return ((~crc) >> 16) & 0x7fff;
}
uint32_t hash_jump_consistent(uint64_t key, int32_t num_buckets)
{

  int64_t b = -1, j = 0;
  uint32_t value = 0;
  while (j < num_buckets)
  {
    b = j;
    key = key * 2862933555777941757ULL + 1;
    j = (b + 1) * ((double)(1LL << 31) / (double)((key >> 33) + 1));
  }
  value = (b < 0) ? (~b + 1) : b;
  return value;
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
inline static lru_list *lru_list_create(lru_cmp_func cmp_func, lru_key_func key_func, lru_val_func val_func)
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
lru_cache *lru_cache_create(uint32_t cap, lru_hash_func hash_func, lru_cmp_func cmp_func, lru_key_func key_func, lru_val_func val_func)
{
  lru_cache *cache = calloc(1, sizeof(lru_cache));
  assert(cache != NULL);
  if (lru_cache_init(cache, cap, hash_func, cmp_func, key_func, val_func) != 0)
  {
    free(cache->tables);
    free(cache);
    cache = NULL;
  }
  return cache;
}

int lru_cache_init(lru_cache *cache, uint32_t cap, lru_hash_func hash_func, lru_cmp_func cmp_func, lru_key_func key_func, lru_val_func val_func)
{
  if (cache == NULL || cap <= 0 || cmp_func == NULL || key_func == NULL || val_func == NULL)
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
  cache->list = lru_list_create(cmp_func, key_func, val_func);
  cache->list->head = cache->list->tail = NULL;
  cache->hash_func = hash_func;
  if (cache->hash_func == NULL)
  {
    cache->hash_func = (lru_hash_func)hash_crc32;
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
  uint32_t hash = cache->hash_func(expire_node->key, expire_node->key_len);
  uint32_t index = hash_jump_consistent(hash, cache->cap);
  lru_list *index_list = cache->tables[index];
  lru_node *remove_node = expire_node->link;
  if (remove_node != NULL)
  {
    lru_list_pop(index_list, remove_node);
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
  __sync_fetch_and_sub(&cache->size, 1);
  return 0;
}
void *lru_cache_put(lru_cache *cache, void *key, uint32_t key_len, void *val)
{
  uint32_t hash = cache->hash_func(key, key_len);
  uint32_t index = hash_jump_consistent(hash, cache->cap);
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
  link_node->link = new_node;
  lru_list_push(index_list, new_node);
  lru_list_push(cache->list, link_node);
  __sync_fetch_and_add(&cache->size, 1);
  return link_node->val;
}
void *lru_cache_get(lru_cache *cache, void *key, uint32_t key_len)
{
  uint32_t hash = cache->hash_func(key, key_len);
  uint32_t index = hash_jump_consistent(hash, cache->cap);
  lru_list *index_list = cache->tables[index];
  lru_node *cache_node = lru_list_find(index_list, key, key_len);
  if (cache_node != NULL)
  {
    lru_node *link = cache_node->link;
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
static void lru_cache_dump_table(lru_cache *cache)
{
  for (int i = 0; i < cache->cap; i++)
  {
    lru_list *list = cache->tables[i];
    if (list != NULL && list->size > 0)
    {
      fprintf(stdout, "level:%d,[ ", i);
      lru_node *cur = list->head;
      while (cur != NULL)
      {
        lru_node *next = cur->next;
        if (next == NULL)
        {
          fprintf(stdout, "{%s,%s}", (char *)cur->key, (char *)cur->val);
        }
        else
        {
          fprintf(stdout, "{%s,%s},", (char *)cur->key, (char *)cur->val);
        }
        cur = next;
      }
      fprintf(stdout, " ]\n");
    }
  }
}
static void lru_cache_dump_list(lru_cache *cache)
{
  fprintf(stdout, "expire:[ ");
  lru_node *cur = cache->list->head;
  while (cache->list->size > 0 && cur != NULL)
  {
    lru_node *next = cur->next;
    if (next == NULL)
    {
      fprintf(stdout, "{%s,%s}", (char *)cur->key, (char *)cur->val);
    }
    else
    {
      fprintf(stdout, "{%s,%s},", (char *)cur->key, (char *)cur->val);
    }
    cur = next;
  }
  fprintf(stdout, "]\n ");
}

static void *lru_cache_list_rand_key(lru_cache *cache)
{
  uint32_t index = hash_jump_consistent(rand(), cache->size);
  if (index == 0)
  {
    index = rand() % cache->size;
  }
  lru_list *list = cache->list;
  lru_node *cur = list->head;
  while (1)
  {
    if (index == 0)
    {
      break;
    }
    cur = cur->next;
    index--;
  }
  return cur->key;
}
#ifdef TEST
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int cmp_func(void *key1, uint32_t key1_len, void *key2, uint32_t key2_len)
{
  const char *k1 = (const char *)key1;
  const char *k2 = (const char *)key2;
  if (key1_len != key2_len)
  {
    return -1;
  }
  return strncmp(k1, k2, key1_len);
}
int value_func(void *val)
{
  if (val != NULL)
  {
    free(val);
    val = NULL;
  }
  return 0;
}
int key_func(void *key)
{
  if (key != NULL)
  {
    free(key);
    key = NULL;
  }
  return 0;
}
int main(void)
{
  int n = 10;
  char *keys[n];
  char *vals[n];
  char *key = malloc(64);
  char *value = malloc(64);
  for (int i = 0; i < n; i++)
  {
    int j = i;
    snprintf(key, 64, "%d", j + rand() % 64);
    keys[i] = strdup(key);
    snprintf(value, 64, "%d", j + rand() % 128);
    vals[i] = strdup(value);
  }
  fprintf(stdout, "----------------------------put operation----------------------\n\n");
  lru_cache *cache = lru_cache_create(4, NULL, cmp_func, key_func, value_func);
  for (int i = 0; i < n; i++)
  {
    lru_cache_put(cache, keys[i], strlen(keys[i]), vals[i]);
    fprintf(stdout, "***************put %s dump**************\n", keys[i]);
    lru_cache_dump_table(cache);
    lru_cache_dump_list(cache);
  }
  fprintf(stdout, "\n----------------------------get operation----------------------\n\n");
  for (int i = 0; i < n; i++)
  {
    char *key = lru_cache_list_rand_key(cache);
    lru_cache_get(cache, key, strlen(key));
    fprintf(stdout, "***************get %s dump**************\n", key);
    lru_cache_dump_table(cache);
    lru_cache_dump_list(cache);
  }
  lru_cache_destroy(cache);
  return 0;
}
#endif