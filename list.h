/*************************************************************************
  > File Name: list.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:38:55 HKT
 ************************************************************************/

#ifndef _LIST_H
#define _LIST_H
#include <pthread.h>
typedef struct list_node  list_node;
typedef struct list_t {
  list_node *head;
  list_node *tail;
  size_t nelem;
  size_t size;
} list;

list *list_create(size_t size);
int list_init(list *lt, size_t size);
void  *list_push_back(list *lt);
void  *list_push_front(list *lt);
void  *list_insert(list *lt,size_t index);
void  *list_remove(list *lt,size_t index);
void *list_pop_back(list *lt);
void  *list_pop_front(list *lt);
void list_release_elem(void *data);
int list_reverse(list *lt);
int list_duplicate(list *dst_lt, list *src_lt);
int list_traverse(list *lt);
void list_free(list *lt);
void list_deinit(list *lt);
#endif
