/*************************************************************************
  > File Name: queue.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Wednesday, July 15, 2020 PM02:30:23 HKT
 ************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H
#include "object.h"
typedef struct cstl_queue_node cstl_queue_node;
typedef struct  cstl_queue {
   cstl_queue_node *head;
   cstl_queue_node *tail;
   size_t  size;
   cstl_object_data_free free;
}cstl_queue;

cstl_queue *cstl_queue_alloc(cstl_object_data_free free);
size_t   cstl_queue_len(cstl_queue *q);
int cstl_queue_push(cstl_queue *q,cstl_object *obj);
cstl_object *cstl_queue_pop(cstl_queue *q);
bool  cstl_queue_is_empty(cstl_queue *q);
void cstl_queue_traverse(cstl_queue *q, cstl_object_cb cb);
void cstl_queue_free(cstl_queue *q);
#endif
