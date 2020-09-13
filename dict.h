/*************************************************************************
  > File Name: dict.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 六  9/12 23:08:14 2020
 ************************************************************************/

#ifndef _DICT_H
#define _DICT_H
#include <stdint.h>
#include <stdio.h>
typedef uint32_t (*dict_hash_fn)(const char *key, size_t len);
typedef struct dict_t
{
  uint64_t max_count;
  uint32_t count;
  void **members;
  dict_hash_fn hash_fn;
} dict;
int dict_init(dict *d, uint32_t max_count, dict_hash_fn hash_fn);
dict *dict_create(uint32_t max_count, dict_hash_fn hash_fn);
void *dict_add(dict *d, const char *key, size_t data_len);
void *dict_get(dict *d, const char *key);
void *dict_del(dict *d, const char *key);
void dict_data_release(void *data);
void dict_deinit(dict *d);
void dict_destroy(dict *d);
#endif
