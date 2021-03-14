/*************************************************************************
  > File Name: list.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 六 12/12 11:44:44 2020
 ************************************************************************/

#ifndef _LIST_H
#define _LIST_H
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
typedef struct list_node_t
{
  struct list_node_t *prev;
  struct list_node_t *next;
  char elem[0];
} list_node;
typedef struct list_t
{
  list_node *head;
  list_node *tail;
  int64_t size;
  uint32_t elem_size;
  int64_t cap;
  pthread_mutex_t lock;
} list;
int list_init(list *lt, uint32_t elem_size, int64_t cap);
list *list_create(uint32_t elem_size, int64_t cap);
int list_push_back(list *lt, void *data);
int list_push_front(list *lt, void *data);
list_node *list_pop_back(list *lt);
list_node *list_pop_front(list *lt);
list_node *list_erase(list *lt, uint32_t index);
void *list_insert(list *lt, void *data, uint32_t index);
int list_deinit(list *lt);
void list_destroy(list *lt);
void list_swap(list *src, list *dst);
void list_reverse(list *lt);
list *list_append(list *src, list *dst);
inline bool list_empty(list *lt)
{
  if (lt != NULL && lt->size > 0)
  {
    return true;
  }
  return false;
}
#endif
