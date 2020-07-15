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
  cstl_object *data;
};
static cstl_queue_node *cstl_queue_node_alloc(cstl_object *obj) {
  cstl_queue_node *node = NULL;
  if (obj != NULL) {
    if ((node = (cstl_queue_node *)calloc(1, sizeof(cstl_queue_node))) ==
        NULL) {
      return node;
    }
    node->data = obj;
  }
  return node;
}
static void cstl_queue_node_free(cstl_queue_node *node,
                                 cstl_object_data_free dfree) {
  if (node != NULL) {
    if (dfree != NULL) {
      cstl_object_free(node->data, dfree);
    }
    free(node);
    node = NULL;
  }
}
cstl_queue *cstl_queue_alloc(cstl_object_data_free dfree) {
  cstl_queue *q = (cstl_queue *)calloc(1, sizeof(cstl_queue));
  if (q != NULL) {
    q->head = q->tail = NULL;
    q->size = 0;
    q->free = dfree;
  }
  return q;
}
size_t cstl_queue_len(cstl_queue *q) {
  if (q != NULL) {
    return q->size;
  }
  return 0;
}
int cstl_queue_push(cstl_queue *q, cstl_object *obj) {
  if (q == NULL || obj == NULL) {
    return -1;
  }
  cstl_queue_node *node = cstl_queue_node_alloc(obj);
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
cstl_object *cstl_queue_pop(cstl_queue *q) {
  cstl_object *obj = NULL;
  if (q != NULL && q->size > 0) {
    cstl_queue_node *node = q->head;
    if (q->head->prev != NULL) {
      cstl_queue_node *prev = q->head->prev;
      q->head = prev;
      q->head->next = NULL;
      node->prev = node->next = NULL;
      obj = node->data;
    }
  }
  return obj;
}
bool cstl_queue_is_empty(cstl_queue *q) {
  if (q->size == 0) {
    return true;
  }
  return false;
}
void cstl_queue_traverse(cstl_queue *q, cstl_object_cb cb) {
  if (cb != NULL) {
    cstl_queue_node *tail = q->tail;
    for (; tail != NULL; tail = tail->next) {
      cstl_object *object = tail->data;
      cb(object);
    }
  }
}
void cstl_queue_free(cstl_queue *q) {

  if (q != NULL) {
    cstl_queue_node *tail = q->tail;
    for (; tail != NULL; tail = tail->next) {
      cstl_queue_node *node = tail;
      cstl_queue_node_free(node, q->free);
      --q->size;
    }
    free(q);
    q = NULL;
  }
}