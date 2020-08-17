/*************************************************************************
  > File Name: cstl_list.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:38:55 HKT
 ************************************************************************/

#ifndef _LIST_H
#define _LIST_H
#include "object.h"
#include <pthread.h>
typedef struct cstl_list_node  cstl_list_node;
typedef struct cstl_list_t {
  cstl_list_node dummy;
  size_t size;
  cstl_object_func *funcs;
} cstl_list;

cstl_list *cstl_list_alloc(cstl_object_func *funcs);
int cstl_list_init(cstl_list *list, cstl_object_func *funcs);
int cstl_list_push_back(cstl_list *list, cstl_object *obj);
int cstl_list_push_front(cstl_list *list, cstl_object *obj);
cstl_object *cstl_list_pop_back(cstl_list *list);
cstl_object *cstl_list_pop_front(cstl_list *list);
int cstl_list_reverse(cstl_list *list);
int cstl_list_duplicate(cstl_list *dst_list, cstl_list *src_list);
int cstl_list_traverse(cstl_list *list);
void cstl_list_free(cstl_list *list);
void cstl_list_deinit(cstl_list *list);

#endif
