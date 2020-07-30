/*************************************************************************
  > File Name: stack.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Tuesday, July 14, 2020 PM05:28:32 HKT
 ************************************************************************/

#ifndef _STACK_H
#define _STACK_H
#include "object.h"
#include <stdint.h>
typedef struct cstl_stack_t
{
  cstl_object **data;
  size_t max_size;
  size_t cur_size;
  cstl_object_func *funcs;
} cstl_stack;

cstl_stack *cstl_stack_alloc(uint64_t init_size, cstl_object_func *funcs);
int cstl_stack_init(cstl_stack *s, uint64_t init_size, cstl_object_func *funcs);
cstl_object *cstl_stack_pop(cstl_stack *s);
cstl_object *cstl_stack_top(cstl_stack *s);
int cstl_stack_push(cstl_stack *s, cstl_object *obj);
int cstl_stack_expand(cstl_stack *s);
int cstl_stack_traverse(cstl_stack *s);
void cstl_stack_free(cstl_stack *s);
void cstl_stack_deinit(cstl_stack *s);

#endif
