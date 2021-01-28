/*************************************************************************
  > File Name: list.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 六 12/12 11:44:50 2020
 ************************************************************************/

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
int list_init(list *lt, int64_t cap, list_push_cb_func push_func, list_pop_cb_func pop_func, list_free_cb_func free_func)
{
  if (lt != NULL)
  {
    lt->cap = cap;
    lt->size = 0;
    lt->push_func = push_func;
    lt->free_func = free_func;
    lt->pop_func = pop_func;
    lt->head = lt->tail = NULL;
    return 0;
  }
  return -1;
}
list *list_create(int64_t cap, list_push_cb_func push_func, list_pop_cb_func pop_func, list_free_cb_func free_func)
{
  list *lt = calloc(1, sizeof(list));
  assert(lt != NULL);
  if (list_init(lt, cap, push_func, pop_func, free_func) != 0)
  {
    free(lt);
    lt = NULL;
  }
  return lt;
}
int list_push_node(list *lt, uint32_t index, list_node *node)
{
  if (lt != NULL)
  {

    if (lt->size == 0)
    {
      lt->head = lt->tail = node;
    }
    else
    {
      if (index == 0)
      {
        lt->head->prev = node;
        node->next = lt->head;
        lt->head = node;
      }
      else if (index == lt->size - 1)
      {
        lt->tail->next = node;
        node->prev = lt->tail;
        lt->tail = node;
      }
      else
      {
        list_node *current = lt->head;
        uint32_t pos = 0;
        while (current != NULL)
        {
          if (pos == index - 1)
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
    if (lt->push_func != NULL)
    {
      lt->push_func(node->data);
    }
    return 0;
  }
  return -1;
}
int list_push_back(list *lt, void *data)
{
  if (lt != NULL && (lt->size < lt->cap || lt->cap == -1))
  {
    list_node *node = list_node_create(data);
    assert(node != NULL);
    return list_push_node(lt, lt->size - 1, node);
  }
  return -1;
}
int list_push_front(list *lt, void *data)
{
  if (lt != NULL && (lt->size < lt->cap || lt->cap == -1))
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
    if (lt->pop_func != NULL)
    {
      lt->pop_func(node->data);
    }
    return 0;
  }
  return -1;
}
int list_push(list *lt, uint32_t index, void *data)
{
  if (lt != NULL && lt->cap != -1 && lt->size >= lt->cap)
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
      if (pos == index - 1)
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
      if (lt->free_func != NULL)
      {
        lt->free_func(current->data);
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
#ifdef TEST
#include <string.h>
static void list_dump(list *lt)
{
  if (lt != NULL && lt->size > 0)
  {
    list_node *current = lt->head;
    fprintf(stdout, "list:%d{ ", lt->size);
    while (current != NULL)
    {
      list_node *next = current->next;
      if (next == NULL)
      {
        fprintf(stdout, "%s", (char *)current->data);
      }
      else
      {
        fprintf(stdout, "%s,", (char *)current->data);
      }
      current = current->next;
    }
    fprintf(stdout, "}\n");
  }
}
int main(void)
{
  list *lt = list_create(20, NULL, NULL, NULL);
  int n = 10;
  char *val[n];
  for (int i = 0; i < n; i++)
  {
    char buf[64] = {'\0'};
    snprintf((char *)&buf, 64, "%d", rand() % 256);
    val[i] = strdup((char *)&buf);

    if (i % 2 == 0)
    {
      fprintf(stdout, "-------------push back index=%d,val=%s-----------\n", i, val[i]);

      list_push_back(lt, val[i]);
    }
    else
    {
      fprintf(stdout, "-------------push front index=%d,val=%s-----------\n", i, val[i]);

      list_push_front(lt, val[i]);
    }
    list_dump(lt);
  }
  char *v = list_pop_back(lt);
  fprintf(stdout, "-------------pop back val=%s-----------\n", v);
  list_dump(lt);

  v = list_pop_front(lt);

  fprintf(stdout, "-------------pop front val=%s-----------\n", v);
  list_dump(lt);
  uint32_t index = rand() % lt->size;
  char buf[64] = {'\0'};
  snprintf((char *)&buf, 64, "%d", rand() % 256);
  v = strdup((char *)&buf);
  list_push(lt, index, v);
  fprintf(stdout, "-------------push  index=%d,val=%s-----------\n", index, v);
  list_dump(lt);

  index = rand() % lt->size;
  snprintf((char *)&buf, 64, "%d", rand() % 256);
  v = strdup((char *)&buf);
  list_push(lt, index, v);
  fprintf(stdout, "-------------push  index=%d,val=%s-----------\n", index, v);
  list_dump(lt);
  index = rand() % lt->size;
  v = list_pop(lt, index);
  fprintf(stdout, "-------------pop  index=%d,val=%s-----------\n", index, v);
  list_dump(lt);
  index = rand() % lt->size;

  v = list_pop(lt, index);
  fprintf(stdout, "-------------pop  index=%d,val=%s-----------\n", index, v);

  list_dump(lt);
  list_destroy(lt);
}
#endif