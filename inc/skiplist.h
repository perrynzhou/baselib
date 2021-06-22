/*
 * Copyright (C) 2021, perrynzhou <perrynzhou@gmail.com>
 */

#ifndef _SKIPLIST_H
#define _SKIPLIST_H
#include "slice.h"
#include <stdint.h>
#include <unistd.h>
#define MAX_LEVEL 32

typedef int (*key_cmp_func)(slice, slice);
typedef struct sk_link {
  struct sk_link *prev, *next;
} sk_link;

typedef struct skiplist {
  int level;
  uint64_t count;
  uint64_t bytes;
  key_cmp_func cmp_func;
  sk_link head[MAX_LEVEL];
} skiplist;

typedef struct skipnode {
  slice key;
  int value;
  sk_link link[0];
} skipnode;

skiplist *skiplist_new(key_cmp_func cmp);
void skiplist_delete(skiplist *list);
skipnode *skiplist_search(skiplist *list, slice key);
skipnode *skiplist_insert(skiplist *list, slice key, int value);
void skiplist_remove(skiplist *list, slice key);
void skiplist_dump(skiplist *list);
#endif /* _SKIPLIST_H */
