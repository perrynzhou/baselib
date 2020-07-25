/*************************************************************************
  > File Name: queue.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 PM02:30:27 HKT
 ************************************************************************/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
struct cstl_queue_node {
  struct cstl_queue_node *next;
  struct cstl_queue_node *prev;
  struct cstl_object *data;
};
static struct cstl_queue_node *cstl_queue_node_alloc(struct cstl_object *obj) {
  struct cstl_queue_node *node = NULL;
  if (obj != NULL) {
    if ((node = (struct cstl_queue_node *)calloc(1, sizeof(struct cstl_queue_node))) ==
        NULL) {
      return node;
    }
    node->data = obj;
  }
  return node;
}
static void cstl_queue_node_free(struct cstl_queue *q,struct cstl_queue_node *node) {
  if (node != NULL) {
    if (q->funcs->object_free_func != NULL) {
      q->funcs->object_free_func(node->data, q->funcs->data_free_func);
    }
    free(node);
    node = NULL;
  }
}
int cstl_queue_init(struct cstl_queue *q, struct cstl_object_func *funcs) {
  if (q != NULL) {
    q->head = q->tail = NULL;
    q->size = 0;
    if (funcs != NULL) {
      q->funcs = calloc(1, sizeof(struct cstl_object_func));
      if (q->funcs == NULL) {
        return -1;
      }
      q->funcs->data_free_func = funcs->data_free_func;
      q->funcs->object_free_func = funcs->object_free_func;
      q->funcs->object_process_func = funcs->object_process_func;
      return 0;
    }
    return -1;
  }
}
struct cstl_queue *cstl_queue_alloc(struct cstl_object_func *funcs) {
 struct  cstl_queue *q = (struct cstl_queue *)calloc(1, sizeof(struct cstl_queue));
  if (cstl_queue_init(q, funcs) != 0) {
    free(q);
    q = NULL;
  }
  return q;
}
size_t cstl_queue_len(struct cstl_queue *q) {
  if (q != NULL) {
    return q->size;
  }
  return 0;
}
int cstl_queue_push(struct cstl_queue *q, struct cstl_object *obj) {
  if (q == NULL || obj == NULL) {
    return -1;
  }
  struct cstl_queue_node *node = cstl_queue_node_alloc(obj);
  if (q->head == NULL) {
    q->head = q->tail = node;
    q->head->next = q->tail->prev = NULL;
  } else {
    node->next = q->tail;
    q->tail->prev = node;
    q->tail = node;
  }
  q->size++;
  return 0;
}
struct cstl_object *cstl_queue_pop(struct cstl_queue *q) {
 struct  cstl_object *obj = NULL;
  if (q != NULL && q->size > 0) {
    struct cstl_queue_node *node = q->head;
    if (q->head->prev != NULL) {
     struct  cstl_queue_node *prev = q->head->prev;
      q->head = prev;
      q->head->next = NULL;
      node->prev = node->next = NULL;
      obj = node->data;
    }
  }
  return obj;
}
bool cstl_queue_is_empty(struct cstl_queue *q) {
  if (q->size == 0) {
    return true;
  }
  return false;
}
void cstl_queue_traverse(struct cstl_queue *q) {
  if (q != NULL && q->size > 0) {
    struct cstl_queue_node *tail = q->tail;
    for (; tail != NULL; tail = tail->next) {
      struct cstl_object *object = tail->data;
      if (q->funcs->object_process_func != NULL) {
        q->funcs->object_process_func(object);
      }
    }
  }
}

void cstl_queue_deinit(struct cstl_queue *q) {
  if (q != NULL) {
   struct cstl_queue_node *tail = q->tail;
    for (; tail != NULL; tail = tail->next) {
     struct cstl_queue_node *node = tail;
      cstl_queue_node_free(q, node);
      --q->size;
    }
  }
}
void cstl_queue_free(struct cstl_queue *q) {

  cstl_queue_deinit(q);
  if (q != NULL) {
    free(q);
    q = NULL;
  }
}