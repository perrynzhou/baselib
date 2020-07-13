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
static cstl_list_node *cstl_list_node_alloc(cstl_object *obj) {
  cstl_list_node *node = NULL;
  if (obj != NULL) {
    node = calloc(1, sizeof(cstl_list_node));
    assert(node != NULL);
    node->next = node->prev = NULL;
    cstl_object_memcpy(&node->data, obj);
  }
  return node;
}
static void cstl_list_node_free(cstl_list_node *node) {
  if (node != NULL) {
    node->prev = node->next = NULL;
    cstl_object_deinit(&node->data);
    free(node);
  }
}
inline static bool cstl_list_valid(cstl_list *list) {
  if (list == NULL || list->size == 0) {
    return false;
  }
  return true;
}
int cstl_list_init(cstl_list *list) {
  if (list == NULL) {
    return -1;
  }
  list = calloc(1, sizeof(cstl_list));
  assert(list != NULL);
  // list->dummy.prev  pointer list head
  // list->dummy.next pointer list tail
  list->dummy.next = list->dummy.prev = NULL;
  list->size = 0;
  pthread_mutex_init(&list->lock, NULL);
  return 0;
}
int cstl_list_insert(cstl_list *list, int pos, cstl_object *obj) {
  if (list == NULL || obj == NULL) {
    return -1;
  }

  pthread_mutex_lock(&list->lock);
  cstl_list_node *node = list_node_alloc(obj);
  assert(node != NULL);

  if (list->dummy.next == NULL) {
    list->dummy.next = list->dummy.prev = node;
  } else {
    if (pos >= 0) {
      cstl_list_node *head = list->dummy.prev;
      node->next = head;
      head->prev = node;
    } else {
      cstl_list_node *tail = list->dummy.next;
      tail->next = node;
      node->prev = tail;
      list->dummy.next = node;
    }
  }
  list->size++;
  pthread_mutex_unlock(&list->lock);
  return 0;
}
int cstl_list_push_back(cstl_list *list, cstl_object *obj) {
  return cstl_list_insert(list, -1, obj);
}
int cstl_list_push_front(cstl_list *list, cstl_object *obj) {
  return cstl_list_insert(list, 0, obj);
}
int cstl_list_remove(cstl_list *list, int pos) {
  if (!cstl_list_valid(list)) {
    return -1;
  }
  cstl_list_node *node = NULL;
  pthread_mutex_lock(&list->lock);

  if (pos >= 0) {
    node = list->dummy.prev;
    if (node == list->dummy.next) {
      list->dummy.prev = list->dummy.next = NULL;
    } else {
      list->dummy.prev = node->next;
    }
  } else {
    node = list->dummy.next;
    if (node == list->dummy.prev) {
      list->dummy.prev = list->dummy.next = NULL;
    } else {
      list->dummy.next = node->prev;
    }
  }
  list->size--;
  cstl_list_node_free(node);
  pthread_mutex_unlock(&list->lock);
  return 0;
}
cstl_object *cstl_list_pop_back(cstl_list *list) {
  return cstl_list_remove(list, -1);
}
cstl_object *cstl_list_pop_front(cstl_list *list) {
  return cstl_list_remove(list, 0);
}

int cstl_list_traverse(cstl_list *list) {
  if (!cstl_list_valid(list)) {
    return -1;
  }
  pthread_mutex_lock(&list->lock);
  cstl_list_node *first = list->dummy.prev;
  cstl_list_node *last = list->dummy.next;
  list->dummy.prev = list->dummy.next = NULL;
  for (; last != NULL; last = last->prev) {
    if (list->dummy.prev == NULL) {
      list->dummy.prev = list->dummy.next = last;
    } else {
      cstl_list_node *tail = list->dummy.next;
      tail->next = last;
      last->prev = tail;
      tail = last;
      list->dummy.next = last;
    }
  }
  pthread_mutex_unlock(&list->lock);
}
void cstl_list_deinit(cstl_list *list) {}