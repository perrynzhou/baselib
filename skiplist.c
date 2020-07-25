/*************************************************************************
  > File Name: skiplist.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Friday, July 17, 2020 AM11:43:40 HKT
 ************************************************************************/

#include "skiplist.h"
#include <stdio.h>

#define CSTL_SKIPLIST_MAXLEVEL 32 /* Should be enough for 2^64 elements */
#define CSTL_SKIPLIST_P 0.25      /* Skiplist P = 1/4 */
struct cstl_skiplist_node {
  int64_t key;
  struct cstl_object *data;
  size_t level;
  struct cstl_skiplist_node *next_nodes[0];
};
int cstl_skiplist_random_level(void) {
  int level = 1;
  while ((random() & 0xFFFF) < (CSTL_SKIPLIST_P * 0xFFFF))
    level += 1;
  return (level < CSTL_SKIPLIST_MAXLEVEL) ? level : CSTL_SKIPLIST_MAXLEVEL;
}
static struct cstl_skiplist_node *
cstl_skiplist_node_alloc(int64_t key, struct cstl_object *obj) {
  struct cstl_skiplist_node *node = NULL;
  if (obj != NULL) {
    int level = cstl_skiplist_random_level();
    node = calloc(1, sizeof(struct cstl_skiplist_node) +
                         sizeof(struct cstl_skiplist_node *) * level);
    node->level = level;
    node->data = obj;
    node->key = key;
    memset(node->next_nodes, 0, sizeof(struct cstl_skiplist_node *) * level);
  }
  return node;
}
static void cstl_skiplist_node_free(struct cstl_skiplist *sl,
                                    struct cstl_skiplist_node *node) {
  if (sl != NULL && node != NULL) {
    if (sl->funcs != NULL && sl->funcs->object_free_func != NULL) {
      sl->funcs->object_free_func(node->data, sl->funcs->data_free_func);
    }
    if (node->next_nodes != NULL) {
      free(node->next_nodes);
    }
    free(node);
    node = NULL;
  }
}
int cstl_skiplist_init(struct cstl_skiplist *sl,
                       struct cstl_object_func *funcs) {
  sl->head = calloc(1, sizeof(struct cstl_skiplist_node) +
                           sizeof(struct cstl_skiplist_node *) *
                               CSTL_SKIPLIST_MAXLEVEL);
  if (sl->head == NULL) {
    return -1;
  }
  sl->head->data = NULL;
  sl->max_level = CSTL_SKIPLIST_MAXLEVEL;
  sl->funcs =
      cstl_object_func_alloc(funcs->data_free_func, funcs->object_free_func,
                             funcs->object_process_func);
  return 0;
}
struct cstl_skiplist *cstl_skiplist_alloc(struct cstl_object_func *funcs) {
  struct cstl_skiplist *sl = calloc(1, sizeof(struct cstl_skiplist));
  if (sl != NULL) {
    if (cstl_skiplist_init(sl, funcs) != 0) {
      free(sl);
      sl = NULL;
    }
  }
  return sl;
}
int cstl_skiplist_push(struct cstl_skiplist *sl, int64_t key,
                       struct cstl_object *obj) {
  struct cstl_skiplist_node *updates[sl->max_level];
  if (sl != NULL && obj != NULL) {
    memset(&updates, 0, sizeof(struct cstl_skiplist_node *) * sl->max_level);
    struct cstl_skiplist_node *cur = sl->head;
    for (size_t i = sl->max_level - 1; i >= 0; i--) {
      if (cur->next_nodes[i] != NULL) {
        if (cur->next_nodes[i]->key > key) {
          updates[i] = cur;
        } else {
          while (cur->next_nodes[i]->key < key) {
            cur = cur->next_nodes[i];
          }
          updates[i] = cur;
        }
      }
    }
    struct cstl_skiplist_node *node = cstl_skiplist_node_alloc(key, obj);
    for (size_t i = 0; i < node->level - 1; i++) {
      node->next_nodes[i] = updates[i]->next_nodes[i];
      updates[i]->next_nodes[i] = node->next_nodes[i];
    }
    return 0;
  }
  return -1;
}
int cstl_skiplist_pop(struct cstl_skiplist *sl, int64_t key) {
  if (sl != NULL) {
    struct cstl_skiplist_node *cur = sl->head;
    struct cstl_skiplist_node *updates[sl->max_level];
    struct cstl_skiplist_node *res = NULL;
    for (size_t i = sl->max_level - 1; i >= 0; i--) {
      if (cur->next_nodes[i] != NULL) {
        if (cur->next_nodes[i]->key > key) {
          updates[i] = NULL;
        } else if (cur->next_nodes[i]->key < key) {
          cur = cur->next_nodes[i];
        } else {
          updates[i] = cur;
          res = cur->next_nodes[i];
        }
      }
    }
    if (res == NULL) {
      return -1;
    }
    struct cstl_skiplist_node *tmp = NULL;
    for (size_t i = 0; i < res->level; i++) {
      if (updates[i] != NULL) {
        tmp = updates[i]->next_nodes[i];
        updates[i]->next_nodes[i] = tmp->next_nodes[i];
      }
    }
    cstl_skiplist_node_free(sl, res);
  }
  return -1;
}
struct cstl_object *cstl_skiplist_find(struct cstl_skiplist *sl, int64_t key) {
  struct cstl_object *obj = NULL;

  if (sl != NULL) {
    struct cstl_skiplist_node *node = sl->head;

    for (size_t i = sl->max_level - 1; i >= 0; i--) {
      if (node->next_nodes[i] != NULL && node->next_nodes[i]->key > key) {
        continue;
      }
      while (node->next_nodes[i] != NULL && node->next_nodes[i]->key < key) {
        node = node->next_nodes[i];
      }
      if (node->next_nodes[i]->key == key) {
        obj = node->next_nodes[i]->data;
        break;
      }
    }
  }
  return obj;
}
void cstl_skip_list_deinit(struct cstl_skiplist *sl) {
  if (sl != NULL) {
    struct cstl_skiplist_node *temp = sl->head;
    for (size_t i = sl->max_level - 1; i >= 0; i--) {
      struct cstl_skiplist_node *cur = temp->next_nodes[i];
      while (cur != NULL) {
        struct cstl_skiplist_node *v = cur->next_nodes[i];
        cstl_skiplist_node_free(sl, cur);
        cur = v;
      }
    }
    cstl_skiplist_node_free(sl, temp);
    temp = NULL;
    sl->head = NULL;
  }
}
void cstl_skip_list_free(struct cstl_skiplist *sl) {
  cstl_skip_list_deinit(sl);
  if (sl != NULL) {
    free(sl);
    sl = NULL;
  }
}
