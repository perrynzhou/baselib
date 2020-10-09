/*************************************************************************
  > File Name: list.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:38:55 HKT
 ************************************************************************/

#ifndef _LIST_H
#define _LIST_H
#include <stdio.h>
typedef void (*list_dump_cb)(void *);
typedef struct list_node_t
{
  struct list_node_t *next;
  struct list_node_t *prev;
  char data[];
} list_node;
typedef struct list_t
{
  list_node *head;
  list_node *tail;
  size_t nelem;
  size_t size;
} list;

list *list_create(size_t size);
size_t list_len(list *lt);
int list_init(list *lt, size_t size);
void *list_push_back(list *lt);
void *list_push_front(list *lt);
void *list_pop_back(list *lt);
void *list_pop_front(list *lt);
int list_release_elem(void *data);
int list_reverse(list *lt);
list *list_dup(list *lt);
void list_destroy(list *lt);
void list_dump(list *lt, list_dump_cb cb);
void list_deinit(list *lt);
#endif
