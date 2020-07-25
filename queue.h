/*************************************************************************
  > File Name: queue.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 PM02:30:23 HKT
 ************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H
#include "object.h"
struct cstl_queue_node cstl_queue_node;
struct cstl_queue {
  struct cstl_queue_node *head;
  struct cstl_queue_node *tail;
  size_t size;
  struct cstl_object_func *funcs;
};

struct cstl_queue *cstl_queue_alloc(struct cstl_object_func *funcs);
int cstl_queue_init(struct cstl_queue *q, struct cstl_object_func *funcs);
size_t cstl_queue_len(struct cstl_queue *q);
int cstl_queue_push(struct cstl_queue *q, struct cstl_object *obj);
struct cstl_object *cstl_queue_pop(struct cstl_queue *q);
bool cstl_queue_is_empty(struct cstl_queue *q);
void cstl_queue_traverse(struct cstl_queue *q);
void cstl_queue_free(struct cstl_queue *q);
void cstl_queue_deinit(struct cstl_queue *q);
#endif
