/*************************************************************************
  > File Name: skiplist.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Friday, July 17, 2020 AM11:43:34 HKT
 ************************************************************************/

#ifndef _SKIPLIST_H
#define _SKIPLIST_H
#include "object.h"
struct cstl_skiplist_node;
struct cstl_skiplist {
  struct cstl_skiplist_node *head;
  size_t max_level;
  struct cstl_object_func *funcs;
};

struct cstl_skiplist *cstl_skiplist_alloc(struct cstl_object_func *funcs);
int cstl_skiplist_init(struct cstl_skiplist *sl,
                       struct cstl_object_func *funcs);
int cstl_skiplist_push(struct cstl_skiplist *sl, int64_t key,
                       struct cstl_object *obj);
int cstl_skiplist_pop(struct cstl_skiplist *sl, int64_t key);
struct cstl_object *cstl_skiplist_find(struct cstl_skiplist *sl, int64_t key);
void cstl_skip_list_free(struct cstl_skiplist *sl);
void cstl_skip_list_deinit(struct cstl_skiplist *sl);
#endif
