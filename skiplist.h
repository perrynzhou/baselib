/*************************************************************************
  > File Name: skiplist.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Friday, July 17, 2020 AM11:43:34 HKT
 ************************************************************************/

#ifndef _SKIPLIST_H
#define _SKIPLIST_H
#include "object.h"
typedef struct cstl_skiplist_node cstl_skiplist_node;
typedef struct cstl_skiplist
{
  cstl_skiplist_node *head;
  size_t max_level;
  cstl_object_func *funcs;
} cstl_skiplist;

cstl_skiplist *cstl_skiplist_alloc(cstl_object_func *funcs);
int cstl_skiplist_init(cstl_skiplist *sl, cstl_object_func *funcs);
int cstl_skiplist_push(cstl_skiplist *sl, int64_t key, cstl_object *obj);
int cstl_skiplist_pop(cstl_skiplist *sl, int64_t key);
cstl_object *cstl_skiplist_find(cstl_skiplist *sl, int64_t key);
void cstl_skip_list_free(cstl_skiplist *sl);
void cstl_skip_list_deinit(cstl_skiplist *sl);
#endif
