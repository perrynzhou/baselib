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
typedef struct list_node_t
{
  list_node *next;
  list_node *prev;
  char data[0];
} list_node;

inline static list_node *list_node_alloc(size_t size)
{
  list_node *node = calloc(1, sizeof(list_node) + sizeof(char) * size);
  node->next = node->prev = NULL;
  return node;
}
static int list_node_free(list *list, list_node *node)
{
  if (node != NULL)
  {
    node->prev = node->next = NULL;
    if (list->funcs != NULL)
    {
      if (list->funcs->data_free_func != NULL)
      {
        list->funcs->data_free_func(cstl_object_data(node->data));
      }
      if (list->funcs->object_free_func != NULL)
      {
        list->funcs->object_free_func(node->data);
      }
    }
    free(node);
  }
}
inline static bool list_empty(list *list)
{
  if (list->size == 0)
  {
    return false;
  }
  return true;
}
inline static bool list_nil(list *list)
{
  if (list == NULL)
  {
    return false;
  }
  return true;
}
list *list_create(size_t size)
{

  list *list = calloc(1, sizeof(list));
  if (list_init(list, size) != 0)
  {
    return NULL;
  }
  return list;
}
int list_init(list *lt, size_t size)
{
  if (lt != NULL)
  {
    // list->dummy.prev  pointer list head
    // list->dummy.next pointer list tail
    lt->head= lt->tail = NULL;
    lt->size = 0;
    lt->nelem = 0;
    return 0;
  }
  return -1;
}
inline static list_node *list_search_node(list *lt, size_t index)
{
  list_node *res = NULL;
  if (lt != NULL && lt->nelem >= index)
  {
    size_t i = 0;
    list_node *tmp = NULL;
    list_node *res = NULL;
    for (tmp = lt->head; tmp != NULL; tmp = tmp->next)
    {
      if (i == index)
      {
        res = tmp;
        break;
      }
      i++;
    }
  }
  return res;
}
void *list_remove(list *lt, size_t index)
{
  void *data = NULL;
  list_node *node = list_node_alloc(lt->size);
  if (node != NULL)
  {
    if (node == lt->dummy.next)
    {
      lt->dummy.next = node->prev;

    } else if (node==lt->dummy.prev){
        lt->dummy.prev = 
    }else {

    }
  }
}
void *list_insert(list *lt, size_t index)
{

  void *data = NULL;
  list_node *tmp = list_search_node(lt, index);
  if (tmp != NULL)
  {
    list_node *node = list_node_alloc(lt->size);
    if (node == NULL)
    {
      return data;
    }
    data = (void *)&node->data;
    if (lt->head == tmp)
    {
      node->prev = lt->head->prev;
      node->next = lt->head;
      lt->head->prev = node;
      lt->head = node;


    }else if (lt->tail == tmp)
    {
      node->next = lt->tail->next;
      node->prev = lt->tail;
      lt->tail->next = node;
      lt->tail = node;
    }else {
    node->next = tmp;
    node->prev = tmp->prev;
    tmp->prev = node;
    }
    lt->nelem++;
  }
}
return data;
}
int list_push_back(list *list, cstl_object *obj)
{
  return list_insert(list, -1, obj);
}
int list_push_front(list *list, cstl_object *obj)
{
  return list_insert(list, 0, obj);
}
list_node *list_remove(list *list, int pos)
{
  if (!list_empty(list) || !list_nil(list))
  {
    return NULL;
  }
  list_node *node = NULL;
  if (pos >= 0)
  {
    node = list->dummy.prev;
    if (node == list->dummy.next)
    {
      list->dummy.prev = list->dummy.next = NULL;
    }
    else
    {
      list->dummy.prev = node->next;
    }
  }
  else
  {
    node = list->dummy.next;
    if (node == list->dummy.prev)
    {
      list->dummy.prev = list->dummy.next = NULL;
    }
    else
    {
      list->dummy.next = node->prev;
    }
  }
  list->size--;
  return node;
}
cstl_object *list_pop_back(list *list)
{
  return list_remove(list, -1);
}
cstl_object *list_pop_front(list *list)
{
  return list_remove(list, 0);
}

int list_reverse(list *list)
{
  if (!list_empty(list) || !list_nil(list))
  {
    return -1;
  }
  list_node *last = list->dummy.next;
  list->dummy.prev = list->dummy.next = NULL;
  for (; last != NULL; last = last->prev)
  {
    if (list->dummy.prev == NULL)
    {
      list->dummy.prev = list->dummy.next = last;
    }
    else
    {
      list_node *tail = list->dummy.next;
      tail->next = last;
      last->prev = tail;
      tail = last;
      list->dummy.next = last;
    }
  }
  return 0;
}
int list_duplicate(list *dst_list, list *src_list)
{
  if (!list_empty(src_list) || !list_nil(src_list) ||
      !list_nil(dst_list))
  {
    return -1;
  }
  list_node *first = src_list->dummy.prev;
  for (; first != NULL; first = first->next)
  {
    if (dst_list->dummy.prev == NULL)
    {
      dst_list->dummy.prev = dst_list->dummy.next = first;
    }
    else
    {
      list_node *tail = dst_list->dummy.next;
      tail->next = first;
      first->prev = tail;
      tail = first;
      dst_list->dummy.next = first;
    }
  }
  return 0;
}
int list_traverse(list *list)
{
  list_node *node = list->dummy.prev;
  for (; node != NULL; node = node->next)
  {
    if (list->funcs != NULL && list->funcs->object_process_func != NULL)
    {
      list->funcs->object_process_func(node->data);
    }
  }
}
void list_deinit(list *list)
{
  if (list != NULL)
  {
    list_node *node = list->dummy.prev;
    for (; node != NULL; node = node->next)
    {
      list_node_free(list, node);
    }
  }
}
void list_free(list *list)
{
  list_deinit(list);
  if (list != NULL)
  {
    free(list);
    list = NULL;
  }
}