/*************************************************************************
  > File Name: stack.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:38:55 HKT
 ************************************************************************/

#ifndef _STACK_H
#define _STACK_H
#include <stdio.h>
typedef void (*stack_dump_cb)(void *);
typedef struct stack_node_t
{
  struct stack_node_t *next;
  struct stack_node_t *prev;
  char data[];
} stack_node;
typedef struct stack_t
{
  stack_node *head;
  stack_node *tail;
  size_t nelem;
  size_t size;
} stack;

stack *stack_create(size_t size);
size_t stack_len(stack *lt);
int stack_init(stack *lt, size_t size);
void *stack_push_back(stack *lt);
void *stack_push_front(stack *lt);
void *stack_pop_back(stack *lt);
void *stack_pop_front(stack *lt);
int stack_release_elem(void *data);
int stack_reverse(stack *lt);
stack *stack_dup(stack *lt);
void stack_destroy(stack *lt);
void stack_dump(stack *lt, stack_dump_cb cb);
void stack_deinit(stack *lt);
#endif
