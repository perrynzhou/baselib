/*************************************************************************
  > File Name: list.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 六 12/12 11:44:50 2020
 ************************************************************************/

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
inline static list_node *list_node_create(void *data)
{
  list_node *node = calloc(1, sizeof(list_node));
  assert(node != NULL);
  node->data = data;
  node->next = node->prev = NULL;
  return node;
}
inline static void list_node_destroy(list_node *node)
{
  if (node != NULL)
  {
    free(node);
    node = NULL;
  }
}
int list_init(list *lt, int64_t cap, list_cb_func func)
{
  if (lt != NULL)
  {
    lt->cap = cap;
    lt->size = 0;
    lt->func = func;
    lt->head = lt->tail = NULL;
    return 0;
  }
  return -1;
}
list *list_create(int64_t cap, list_cb_func func)
{
  list *lt = calloc(1, sizeof(list));
  assert(lt != NULL);
  if (list_init(lt, cap, func) != 0)
  {
    free(lt);
    lt = NULL;
  }
  return lt;
}
int list_push_node(list *lt, uint32_t index, list_node *node)
{
  if (lt != NULL && lt->size > 0)
  {

    if (lt->size == 0)
    {
      lt->head = lt->tail = node;
    }
    else
    {
      if (index == 0)
      {
        lt->tail->next = node;
        node->prev = lt->tail;
        lt->tail = node;
      }
      else if (index == lt->size - 1)
      {
        lt->head->prev = node;
        node->next = lt->head;
        lt->head = node;
      }
      else
      {
        list_node *current = lt->head;
        uint32_t pos = 0;
        while (current != NULL)
        {
          if (pos == index)
          {
            break;
          }
          current = current->next;
          pos++;
        }
        node->prev = current->prev;
        if (current->prev != NULL)
        {
          current->prev->next = node;
        }
        node->next = current;
        current->prev = node;
      }
    }
    __sync_fetch_and_add(&lt->size, 1);
    return 0;
  }
  return -1;
}
int list_push_back(list *lt, void *data)
{
  if (lt != NULL && lt->size < lt->cap)
  {
    list_node *node = list_node_create(data);
    assert(node != NULL);
    return list_push_node(lt, lt->size - 1, node);
  }
  return -1;
}
int list_push_front(list *lt, void *data)
{
  if (lt != NULL && lt->size < lt->cap)
  {

    list_node *node = list_node_create(data);
    assert(node != NULL);
    return list_push_node(lt, 0, node);
  }
  return -1;
}
int list_pop_node(list *lt, list_node *node)
{
  if (lt != NULL && lt->size > 0)
  {
    if (node->prev != NULL)
    {
      node->prev->next = node->next;
    }
    else
    {
      lt->head = node->next;
    }
    if (node->next != NULL)
    {
      node->next->prev = node->prev;
    }
    else
    {
      lt->tail = node->prev;
    }
    node->prev = node->next = NULL;
    __sync_fetch_and_sub(&lt->size, 1);
    return 0;
  }
  return -1;
}
int list_push(list *lt, uint32_t index, void *data)
{
  if (lt != NULL && lt->size >= lt->cap)
  {
    return -1;
  }
  if (index <= lt->size)
  {
    list_node *node = list_node_create(data);
    assert(node != NULL);
    if (index == 0)
    {
      return list_push_node(lt, 0, node);
    }
    else if (index == lt->size - 1)
    {
      return list_push_node(lt, lt->size - 1, node);
    }
    else
    {
      return list_push_node(lt, index, node);
    }
  }
  return -1;
}
void *list_pop_back(list *lt)
{
  list_node *tail = lt->tail;
  list_pop_node(lt, tail);
  return tail->data;
}
void *list_pop_front(list *lt)
{
  list_node *head = lt->head;
  list_pop_node(lt, head);
  return head->data;
}
void *list_pop(list *lt, uint32_t index)
{
  if (lt != NULL && lt->size > 0 && index <= lt->size - 1)
  {
    list_node *current = lt->head;
    uint32_t pos = 0;
    while (current != NULL)
    {
      if (pos == index)
      {
        break;
      }
      current = current->next;
      pos++;
    }
    list_pop_node(lt, current);
    return current->data;
  }
  return NULL;
}
int list_deinit(list *lt)
{
  if (lt != NULL)
  {
    list_node *current = lt->head;
    while (current != NULL)
    {
      if (lt->func != NULL)
      {
        lt->func(current->data);
      }
      list_node_destroy(current);
      current = current->next;
    }
  }
}
void list_destroy(list *lt)
{
  if (list_deinit(lt) == 0)
  {
    free(lt);
    lt = NULL;
  }
}
