/*************************************************************************
  > File Name: list.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:39:00 HKT
 ************************************************************************/

#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
inline static list_node *list_node_alloc(size_t size)
{
  list_node *node = calloc(1, size + sizeof(list_node));
  if (node != NULL)
  {
    node->next = node->prev = NULL;
  }
  return node;
}
inline static int list_node_free(void *data)
{
  if (data != NULL)
  {
    list_node *node = (void *)(data - sizeof(list_node));
    free(node);
    return 0;
  }
  return -1;
}
size_t list_len(list *lt)
{
  if (lt != NULL)
  {
    return lt->nelem;
  }
  return 0;
}
list *list_create(size_t size)
{

  list *lt = calloc(1, sizeof(list));
  if (list_init(lt, size) != 0)
  {
    return NULL;
  }
  return lt;
}
int list_init(list *lt, size_t size)
{
  if (lt != NULL)
  {
    lt->head = lt->tail = NULL;
    lt->size = size;
    lt->nelem = 0;
    return 0;
  }
  return -1;
}
inline static list_node *list_search_node(list *lt, size_t index)
{
  list_node *res = NULL;
  if (lt->nelem == 0)
  {
    return NULL;
  }
  if (index > 0 && index == (lt->nelem - 1))
  {
    return lt->tail;
  }
  size_t i = 0;
  list_node *tmp = lt->head;
  while (tmp != NULL)
  {
    if (index == i)
    {
      res = tmp;
      break;
    }
    i++;
    tmp = tmp->next;
  }
  return res;
}
void *list_remove(list *lt, size_t index)
{
  void *data = NULL;
  list_node *tmp = list_search_node(lt, index);
  if (tmp != NULL)
  {
    if (tmp->prev != NULL)
    {
      tmp->prev->next = tmp->next;
    }
    else
    {
      lt->head = tmp->next;
    }
    if (tmp->next != NULL)
    {
      tmp->next->prev = tmp->prev;
    }
    else
    {
      lt->tail = tmp->prev;
    }
    __sync_fetch_and_sub(&lt->nelem, 1);
  }
  return data;
}
void *list_insert(list *lt, size_t index)
{
  void *data = NULL;
  list_node *node = list_node_alloc(lt->size);
  if (node == NULL)
  {
    return data;
  }
  data = &node->data;
  if (lt->nelem == 0)
  {
    lt->head = lt->tail = node;
    __sync_fetch_and_add(&lt->nelem, 1);
    return data;
  }

  list_node *tmp = list_search_node(lt, index);
  if (tmp == NULL)
  {
    node->prev = lt->tail;
    lt->tail->next = node;
    lt->tail = node;
    __sync_fetch_and_add(&lt->nelem, 1);
    return data;
  }
  node->prev = tmp->prev;
  if (tmp->prev != NULL)
  {
    tmp->prev->next = node;
  }
  node->next = tmp;
  tmp->prev = node;
  if (!node->prev)
  {
    lt->head = node;
  }
  if (!node->next)
  {
    lt->tail = node;
  }
  __sync_fetch_and_add(&lt->nelem, 1);
  return data;
}

void *list_push_back(list *lt)
{

  void *data = NULL;
  list_node *node = list_node_alloc(lt->size);
  if (node == NULL)
  {
    return data;
  }
  data = (void *)&node->data;
  if (lt->nelem == 0)
  {
    lt->head = lt->tail = node;
  }
  else
  {
    node->prev = lt->tail;
    lt->tail->next = node;
    lt->tail = node;
  }
  __sync_fetch_and_add(&lt->nelem, 1);
  return data;
}
void *list_push_front(list *lt)
{
  void *data = NULL;
  list_node *node = list_node_alloc(lt->size);
  if (node == NULL)
  {
    return data;
  }
  data = (void *)&node->data;
  if (lt->nelem == 0)
  {
    lt->head = lt->tail = node;
  }
  else
  {
    node->next = lt->head;
    lt->head->prev = node;
    lt->head = node;
  }
  __sync_fetch_and_add(&lt->nelem, 1);
  return data;
}
void *list_pop_back(list *lt)
{
  if (lt->nelem == 0||lt->tail==NULL)
  {
    return NULL;
  }
  list_node *node = lt->tail;
  if (node->prev != NULL)
  {
    lt->tail = node->prev;
    lt->tail->next = NULL;
  }
  node->prev = node->next = NULL;
  __sync_fetch_and_sub(&lt->nelem, 1);
  return (void *)&node->data;
}
void *list_pop_front(list *lt)
{
  if (lt->nelem == 0||lt->head==NULL)
  {
    return NULL;
  }
  list_node *node = lt->head;
  if (node->next != NULL)
  {
    lt->head = node->next;
    lt->head->prev = NULL;
  }
  node->prev = node->next = NULL;
  __sync_fetch_and_sub(&lt->nelem, 1);
  if (lt->nelem == 0)
  {
    lt->head = lt->tail = NULL;
  }
  return (void *)&node->data;
}
int list_release_elem(void *data)
{
  return list_node_free(data);
}
int list_reverse(list *lt)
{
  if (lt != NULL && lt->nelem > 0)
  {
    list_node *tmp = lt->tail;
    list_node *new_head = tmp;
    list_node *new_tail = tmp;
    tmp = tmp->prev;
    new_head->prev = NULL;
    new_head->next = NULL;
    while (tmp != NULL)
    {
      list_node *prev = tmp->prev;
      tmp->next = tmp->prev = NULL;
      new_tail->next = tmp;
      tmp->prev = new_tail;
      new_tail = tmp;
      tmp = prev;
    }
    lt->head = new_head;
    lt->tail = new_tail;
  }
  return 0;
}
void list_dump(list *lt, list_dump_cb cb)
{
  if (lt != NULL && lt->nelem > 0 && cb != NULL)
  {
    list_node *tmp = lt->head;
    while (tmp != NULL)
    {
      void *data = (void *)&tmp->data;
      cb(data);
      tmp = tmp->next;
    }
  }
}
list *list_dup(list *lt)
{
  if (lt->nelem == 0)
  {
    return NULL;
  }
  list_node *node = lt->head;
  list *new_lt = list_create(lt->size);
  for (size_t i = 0; i < lt->nelem; i++)
  {
    list_node *new_node = list_node_alloc(new_lt->size);
    memcpy((void *)&new_node->data, &node->data, new_lt->size);
    if (new_lt->nelem == 0)
    {
      new_lt->head = new_lt->tail = new_node;
    }
    else
    {
      new_node->prev = new_lt->tail;
      new_lt->tail->next = new_node;
      new_lt->tail = new_node;
    }
    __sync_fetch_and_sub(&new_lt->nelem, 1);
    node = node->next;
  }
  return new_lt;
}
void list_deinit(list *lt)
{
  if (lt != NULL && lt->size > 0)
  {
    list_node *node = lt->head;
    for (node = lt->head; node != NULL; node = node->next)
    {
      free(node);
      __sync_fetch_and_sub(&lt->nelem, 1);
    }
    lt->nelem = 0;
  }
}
void list_destroy(list *lt)
{
  list_deinit(lt);
  if (lt != NULL)
  {
    free(lt);
    lt = NULL;
  }
}