/*************************************************************************
  > File Name: queue.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 PM02:30:23 HKT
 ************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H
#include <stdbool.h>
typedef struct queue_node_t queue_node;
typedef struct queue_t
{
  queue_node *head;
  queue_node *tail;
  size_t nelem;
  size_t size;
} queue;

queue *queue_alloc(size_t nelem,size_t size);
int queue_init(queue *q, size_t nelem,size_t size);
size_t queue_len(queue *q);
void *queue_push(queue *q);
void  *queue_pop(queue *q);
void queue_releae_elem(void *data);
bool queue_is_empty(queue *q);
void queue_free(queue *q);
void queue_deinit(queue *q);
#endif
