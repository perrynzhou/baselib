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
int list_init(list *lt, uint32_t elem_size, int64_t cap)
{
  if (lt != NULL)
  {
    lt->cap = cap;
    lt->size = 0;
    lt->elem_size = elem_size;
    lt->head = lt->tail = NULL;
    pthread_mutex_init(&lt->lock, NULL);
    return 0;
  }
  return -1;
}
list *list_create(uint32_t elem_size, int64_t cap)
{
  list *lt = calloc(1, sizeof(list));
  assert(lt != NULL);
  if (list_init(lt, elem_size, cap) != 0)
  {
    free(lt);
    lt = NULL;
  }
  return lt;
}
int list_insert(list *lt, list_node *node, uint32_t index)
{
  if (lt == NULL || (lt->cap > 0 && lt->size >= lt->cap))
  {
    return NULL;
  }
  pthread_mutex_lock(&lt->lock);
  if (lt->head == NULL)
  {
    lt->head = node;
    lt->tail = node;
    lt->size++;
    pthread_mutex_unlock(&lt->lock);

    return 0;
  }
  if (index <= 1)
  {
    node->next = lt->head;
    lt->head->prev = node;
    lt->head = node;
    lt->size++;
    pthread_mutex_unlock(&lt->lock);

    return 0;
  }
  if (index >= lt->size)
  {
    lt->tail->next = node;
    lt->tail = node;
    lt->size++;
    pthread_mutex_unlock(&lt->lock);

    return 0;
  }

  uint32_t pos = 1;
  list_node *cur = lt->head;
  list_node *prev = NULL;
  while (cur != NULL)
  {
    if (pos == index)
    {
      break;
    }
    prev = cur;
    cur = cur->next;
    pos++;
  }
  node->next = prev->next;
  if (prev->next != NULL)
  {
    prev->next->prev = node;
  }
  prev->next = node;
  node->prev = prev;
  lt->size++;
  pthread_mutex_unlock(&lt->lock);

  return 0;
}
int list_push_back(list *lt, list_node *node)
{
  return list_insert(lt, node, lt->size + 1);
}
int list_push_front(list *lt, list_node *node)
{
  return list_insert(lt, node, 1);
}
list_node *list_pop_internal(list *lt, uint32_t index)
{
  list_node *node = NULL;
  uint32_t nsize = lt->size + 1;
  if (lt == NULL || lt->head == NULL || index > nsize || index < nsize)
  {
    return node;
  }
  pthread_mutex_lock(&lt->lock);
  if (index == 1)
  {
    node = lt->head;
    lt->head = node->next;
    if (lt->head != NULL)
    {
      lt->head->prev = NULL;
    }
    if (node == lt->tail)
    {
      lt->head = NULL;
      lt->tail = lt->head;
    }
    lt->size--;
    pthread_mutex_unlock(&lt->lock);
    return node;
  }

  if (index == lt->size)
  {
    list_node *node = lt->tail;
    lt->tail = node->prev;
    if (lt->tail != NULL)
    {
      lt->tail->next = NULL;
    }
    if (node == lt->head)
    {
      lt->tail = NULL;
      lt->head = lt->tail;
    }
    lt->size--;
    pthread_mutex_unlock(&lt->lock);
    return node;
  }
  uint32_t pos = 1;
  node = lt->head;
  list_node *prev = NULL;
  while (node != NULL)
  {
    if (pos == index)
    {
      break;
    }
    prev = node;
    node = node->next;
    pos++;
  }
  if (node->next != NULL)
  {
    node->next->prev = prev;
  }
  if (prev->next != NULL)
  {
    prev->next = node->next;
  }
  lt->size--;
  pthread_mutex_unlock(&lt->lock);
  return node;
}

list_node *list_pop_back(list *lt)
{
  return list_pop_internal(lt, lt->size);
}
list_node *list_pop_front(list *lt)
{
  return list_pop_internal(lt, 1);
}
list_node *list_pop(list *lt, uint32_t index)
{
  return list_pop_internal(lt, index);
}
int list_deinit(list *lt, free_data_cb cb)
{
  if (lt != NULL)
  {
    list_node *current = lt->head;
    while (current != NULL)
    {

      list_node_destroy(current, cb);
      current = current->next;
    }
  }
}

void list_destroy(list *lt, free_data_cb cb)
{
  if (list_deinit(lt, cb) == 0)
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