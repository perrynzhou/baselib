/*************************************************************************
  > File Name: queue.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 PM02:30:27 HKT
 ************************************************************************/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct queue_node_t
{
  struct queue_node_t *next;
  char data[0];
} queue_node;
int queue_init(queue *q, size_t size)
{
  if (q != NULL)
  {
    q->head = q->tail = NULL;
    q->size = size;
    q->nelem = 0;
    return 0;
  }
  return -1;
}
queue *queue_create(size_t size)
{
  queue *q = (queue *)calloc(1, sizeof(queue));
  if (queue_init(q, size) != 0)
  {
    free(q);
    q = NULL;
  }
  return q;
}
size_t queue_len(queue *q)
{
  if (q != NULL)
  {
    return q->nelem;
  }
  return 0;
}
void *queue_push(queue *q)
{
  void *data = NULL;
  if (q != NULL)
  {
    queue_node *node = (queue_node *)calloc(1, sizeof(queue_node) + sizeof(char) * q->size);
    if (q->head == NULL)
    {
      q->head = q->tail = node;
      q->head->next = q->tail->next = NULL;
    }
    else
    {
      q->tail->next = node;
      q->tail = node;
    }
    q->nelem++;
    data = &node->data;
  }
  return data;
}
void *queue_pop(queue *q)
{
  void *data = NULL;
  if (q != NULL && q->nelem > 0)
  {
    queue_node *node = q->head;
    data = node->data;
    if(node!=NULL) {
      q->head = node->next;
    }
    q->nelem--;
  }
  return data;
}
void queue_releae_elem(void *data)
{
  queue_node *node = (void *)data - sizeof(queue_node);
  free(node);
  node = NULL;
}
bool queue_is_empty(queue *q)
{
  if (q->nelem == 0)
  {
    return true;
  }
  return false;
}

void queue_deinit(queue *q)
{
  if (q != NULL)
  {
    queue_node *node = q->head;
    for (node = q->head; node != NULL; node = node->next)
    {
      free(node);
      q->nelem--;
    }
  }
}
void queue_destroy(queue *q)
{
  queue_deinit(q);
  if (q != NULL)
  {
    free(q);
    q = NULL;
  }
}