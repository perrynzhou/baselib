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
struct cstl_list_node {
  cstl_object *data;
  struct cstl_list_node *next;
  struct cstl_list_node *prev;
};

static cstl_list_node *cstl_list_node_alloc(cstl_object *obj) {
  cstl_list_node *node = NULL;
  if (obj != NULL) {
    node = calloc(1, sizeof(cstl_list_node));
    assert(node != NULL);
    node->next = node->prev = NULL;
    node->data = obj;
  }
  return node;
}
static int cstl_list_node_free(cstl_list_node *node, cstl_object_data_free cb) {
  if (node != NULL) {
    node->prev = node->next = NULL;
    cstl_object_free(&node->data, cb);
    free(node);
  }
}
inline static bool cstl_list_empty(cstl_list *list) {
  if (list->size == 0) {
    return false;
  }
  return true;
}
inline static bool cstl_list_nil(cstl_list *list) {
  if (list == NULL) {
    return false;
  }
  return true;
}
int cstl_list_alloc(cstl_list *list, cstl_object_data_free free) {
  if (list == NULL) {
    return -1;
  }
  list = calloc(1, sizeof(cstl_list));
  assert(list != NULL);
  // list->dummy.prev  pointer list head
  // list->dummy.next pointer list tail
  list->dummy.next = list->dummy.prev = NULL;
  list->size = 0;
  list->free = free;
  return 0;
}
int cstl_list_insert(cstl_list *list, int pos, cstl_object *obj) {
  if (!cstl_list_nil(list) || !cstl_object_nil(obj)) {
    return -1;
  }

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
  return 0;
}
int cstl_list_push_back(cstl_list *list, cstl_object *obj) {
  return cstl_list_insert(list, -1, obj);
}
int cstl_list_push_front(cstl_list *list, cstl_object *obj) {
  return cstl_list_insert(list, 0, obj);
}
cstl_list_node *cstl_list_remove(cstl_list *list, int pos) {
  if (!cstl_list_empty(list) || !cstl_list_nil(list)) {
    return NULL;
  }
  cstl_list_node *node = NULL;
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
  return node;
}
cstl_object *cstl_list_pop_back(cstl_list *list) {
  return cstl_list_remove(list, -1);
}
cstl_object *cstl_list_pop_front(cstl_list *list) {
  return cstl_list_remove(list, 0);
}

int cstl_list_reverse(cstl_list *list) {
  if (!cstl_list_empty(list) || !cstl_list_nil(list)) {
    return -1;
  }
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
  return 0;
}
int cstl_list_duplicate(cstl_list *dst_list, cstl_list *src_list) {
  if (!cstl_list_empty(src_list) || !cstl_list_nil(src_list) ||
      !cstl_list_nil(dst_list)) {
    return -1;
  }
  cstl_list_node *first = src_list->dummy.prev;
  for (; first != NULL; first = first->next) {
    if (dst_list->dummy.prev == NULL) {
      dst_list->dummy.prev = dst_list->dummy.next = first;
    } else {
      cstl_list_node *tail = dst_list->dummy.next;
      tail->next = first;
      first->prev = tail;
      tail = first;
      dst_list->dummy.next = first;
    }
  }
  return 0;
}
int cstl_list_traverse(cstl_list *list, cstl_object_cb cb) {
  cstl_list_node *node = list->dummy.prev;
  for (; node != NULL; node = node->next) {
    if (cb != NULL) {
      cb(node->data);
    }
  }
}
void cstl_list_free(cstl_list *list) {
  if (list != NULL) {
    cstl_list_node *node = list->dummy.prev;
    for (; node != NULL; node = node->next) {
      cstl_list_node_free(node, list->free);
    }
  }
}