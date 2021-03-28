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
typedef void (*free_data_cb)(void *);
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
int list_push_back(list *lt, list_node *node);
int list_push_front(list *lt, list_node *node);
list_node *list_pop_back(list *lt);
list_node *list_pop_front(list *lt);
int list_push(list *lt, list_node *node,uint32_t index);
list_node *list_pop(list *lt, uint32_t index);
int list_deinit(list *lt, free_data_cb cb);
void list_destroy(list *lt, free_data_cb cb);
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

inline list_node *list_node_create(void *data, size_t d_size)
{
  list_node *node = NULL;
  if (data != NULL)
  {
    node = (list_node *)calloc(1, sizeof(list_node) + sizeof(char) * d_size);
    assert(node != NULL);
    memcpy(node->elem, data, d_size);
  }
  return node;
}
inline void *list_node_data(list_node *node)
{
  void *data = NULL;
  if (node != NULL)
  {
    data = node->elem;
  }
  return data;
}
inline void list_node_destroy(list_node *node, free_data_cb cb)
{
  if (node != NULL)
  {
    if (cb != NULL)
    {
      cb(node->elem);
    }
    free(node);
    node = NULL;
  }
}
#endif
