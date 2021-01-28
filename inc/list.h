/*************************************************************************
  > File Name: list.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 六 12/12 11:44:44 2020
 ************************************************************************/

#ifndef _LIST_H
#define _LIST_H
#include <stdint.h>
typedef int (*list_push_cb_func)(void *);
typedef int (*list_pop_cb_func)(void *);
typedef int (*list_free_cb_func)(void *);

typedef struct list_node_t
{
  void *data;
  struct list_node_t *prev;
  struct list_node_t *next;
} list_node;
typedef struct list_t
{
  list_node *head;
  list_node *tail;
  uint32_t size;
  int64_t cap;
  list_push_cb_func push_func;
  list_pop_cb_func  pop_func;
  list_free_cb_func free_func;
} list;
int list_init(list *lt, int64_t cap,list_push_cb_func push_func,list_pop_cb_func  pop_func,list_free_cb_func free_func);
list *list_create(int64_t cap,list_push_cb_func push_func,list_pop_cb_func  pop_func,list_free_cb_func free_func);
int list_push_back(list *lt, void *data);
int list_push_front(list *lt, void *data);
int list_push(list *lt, uint32_t index,void *data);
void *list_pop_back(list *lt);
void *list_pop_front(list *lt);
void *list_pop(list *lt, uint32_t index);
int list_deinit(list *lt);
void list_destroy(list *lt);
#endif
