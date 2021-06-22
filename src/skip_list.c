/*
 * Copyright (C) 2015, Leo Ma <begeekmyfriend@gmail.com>
 */

#include "skiplist.h"
#include "slice.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define list_entry(ptr, type, member)                                          \
  ((type *)((char *)(ptr) - (size_t)(&((type *)0)->member)))

#define skiplist_foreach(pos, end) for (; pos != end; pos = pos->next)

#define skiplist_foreach_safe(pos, n, end)                                     \
  for (n = pos->next; pos != end; pos = n, n = pos->next)

static inline void list_init(sk_link *link) {
  link->prev = link;
  link->next = link;
}

static inline void __list_add(sk_link *link, sk_link *prev, sk_link *next) {
  link->next = next;
  link->prev = prev;
  next->prev = link;
  prev->next = link;
}

static inline void __list_del(sk_link *prev, sk_link *next) {
  prev->next = next;
  next->prev = prev;
}

static inline void list_add(sk_link *link, sk_link *prev) {
  __list_add(link, prev, prev->next);
}

static inline void list_del(sk_link *link) {
  __list_del(link->prev, link->next);
  list_init(link);
}

static inline int list_empty(sk_link *link) { return link->next == link; }

static skipnode *skipnode_new(int level, slice key, int value) {
  skipnode *node;
  node = malloc(sizeof(*node) + level * sizeof(struct sk_link));
  if (node != NULL) {
    slice_init(&node->key, (char *)key);
    // node->key = slice_create((const char *)key);
    node->value = value;
  }
  return node;
}

static void skipnode_delete(skipnode *node) { free(node); }

skiplist *skiplist_new(key_cmp_func cmp) {
  int i;
  skiplist *list = malloc(sizeof(*list));
  if (list != NULL) {
    list->level = 1;
    list->count = 0;
    for (i = 0; i < sizeof(list->head) / sizeof(list->head[0]); i++) {
      list_init(&list->head[i]);
    }
  }
  list->cmp_func = cmp;
  return list;
}

void skiplist_delete(skiplist *list) {
  sk_link *n;
  sk_link *pos = list->head[0].next;
  skiplist_foreach_safe(pos, n, &list->head[0]) {
    skipnode *node = list_entry(pos, struct skipnode, link[0]);
    skipnode_delete(node);
  }
  free(list);
}

static int random_level(void) {
  int level = 1;
  const double p = 0.25;
  while ((random() & 0xffff) < 0xffff * p) {
    level++;
  }
  return level > MAX_LEVEL ? MAX_LEVEL : level;
}

skipnode *skiplist_search(skiplist *list, slice key) {
  skipnode *node = NULL;
  int i = list->level - 1;
  sk_link *pos = &list->head[i];
  sk_link *end = &list->head[i];

  for (; i >= 0; i--) {
    pos = pos->next;
    skiplist_foreach(pos, end) {
      node = list_entry(pos, struct skipnode, link[i]);
      if (list->cmp_func(node->key, key) >= 0) {
        end = &node->link[i];
        break;
      }
    }
    if (list->cmp_func(node->key, key) == 0) {
      return node;
    }
    pos = end->prev;
    pos--;
    end--;
  }

  return NULL;
}

skipnode *skiplist_insert(struct skiplist *list, slice key, int value) {
  int level = random_level();
  if (level > list->level) {
    list->level = level;
  }

  skipnode *node = skipnode_new(level, key, value);
  if (node != NULL) {
    int i = list->level - 1;
    sk_link *pos = &list->head[i];
    sk_link *end = &list->head[i];

    for (; i >= 0; i--) {
      pos = pos->next;
      skiplist_foreach(pos, end) {
        skipnode *nd = list_entry(pos, struct skipnode, link[i]);
        if (list->cmp_func(nd->key, key) >= 0) {
          end = &nd->link[i];
          break;
        }
      }
      pos = end->prev;
      if (i < level) {
        __list_add(&node->link[i], pos, end);
      }
      pos--;
      end--;
    }

    list->count++;
  }
  return node;
}

static void __remove(skiplist *list, skipnode *node, int level) {
  int i;
  for (i = 0; i < level; i++) {
    list_del(&node->link[i]);
    if (list_empty(&list->head[i])) {
      list->level--;
    }
  }
  skipnode_delete(node);
  list->count--;
}

void skiplist_remove(skiplist *list, slice key) {
  sk_link *n;
  skipnode *node;
  int i = list->level - 1;
  sk_link *pos = &list->head[i];
  sk_link *end = &list->head[i];

  for (; i >= 0; i--) {
    pos = pos->next;
    skiplist_foreach_safe(pos, n, end) {
      node = list_entry(pos, struct skipnode, link[i]);
      if (list->cmp_func(node->key, key) >= 0) {
        end = &node->link[i];
        break;
      } else if (list->cmp_func(node->key, key) == 0) {
        /* we allow nodes with same key. */
        __remove(list, node, i + 1);
      }
    }
    pos = end->prev;
    pos--;
    end--;
  }
}

void skiplist_dump(skiplist *list) {
  skipnode *node;
  int i = list->level - 1;
  sk_link *pos = &list->head[i];
  sk_link *end = &list->head[i];

  printf("\nTotal %ld nodes: \n", list->count);
  for (; i >= 0; i--) {
    pos = pos->next;
    printf("level %d:\n", i + 1);
    skiplist_foreach(pos, end) {
      node = list_entry(pos, struct skipnode, link[i]);
      printf("key:%s value:%0d\n", (const char *)node->key, node->value);
    }
    pos = &list->head[i];
    pos--;
    end--;
  }
}
