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
#include <assert.h>
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
static void *list_push(list *lt, int direction)
{
  void *data = NULL;

  list_node *node = NULL;
  if ((node = list_node_alloc(lt->size)) == NULL)
  {
    return NULL;
  }
  data = (void *)&node->data;
  if (lt->nelem == 0)
  {
    lt->head = lt->tail = node;
    node->prev = node->next = NULL;
  }
  else
  {
    if (direction == 1)
    {
      node->prev = lt->tail;
      if (lt->tail != NULL)
      {
        lt->tail->next = node;
        lt->tail = node;
      }
    }
    else
    {
      node->next = lt->head;
      if (lt->head != NULL)
      {
        lt->head->prev = node;
        lt->head = node;
      }
    }
  }
  __sync_fetch_and_add(&lt->nelem, 1);
  return data;
}
void *list_push_back(list *lt)
{

  return list_push(lt, 1);
}
void *list_push_front(list *lt)
{
  return list_push(lt, 0);
}
static void *list_pop(list *lt, int direction)
{
  if (lt->nelem == 0)
  {
    return NULL;
  }
  void *data = NULL;

  list_node *node = NULL;
  if (direction == 1)
  {

    node = lt->tail;
    if (node->prev != NULL)
    {
      lt->tail = node->prev;
      lt->tail->next = NULL;
    }
  }
  else
  {
    node = lt->head;
    if (node->next != NULL)
    {
      lt->head = node->next;
      lt->head->prev = NULL;
    }
  }
  __sync_fetch_and_sub(&lt->nelem, 1);
  if (lt->nelem == 0)
  {
    lt->head = lt->tail = NULL;
  }
  node->prev = node->next = NULL;
  data = &node->data;
  return data;
}
void *list_pop_back(list *lt)
{
  return list_pop(lt, 1);
}
void *list_pop_front(list *lt)
{
  return list_pop(lt, 0);
}
int list_release_elem(void *data)
{
  return list_node_free(data);
}
int list_reverse(list *lt)
{
  if (lt->nelem == 0)
  {
    return -1;
  }

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

  return 0;
}
void list_dump(list *lt, list_dump_cb cb)
{

  if (lt->nelem > 0)
  {
    list_node *tmp = lt->head;
    while (tmp != NULL)
    {
      void *data = (void *)&tmp->data;
      if (cb != NULL)
      {
        cb(data);
      }
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
  if (lt->nelem > 0)
  {
    list_node *node = lt->head;
    for (node = lt->head; node != NULL; node = node->next)
    {
      free(node);
      __sync_fetch_and_sub(&lt->nelem, 1);
    }
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