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
struct cstl_stack {
  struct cstl_object **data;
   size_t max_size;
   size_t cur_size;
   struct cstl_object_func *funcs;
};

struct cstl_stack *cstl_stack_alloc(uint64_t init_size,struct cstl_object_func *funcs);
int cstl_stack_init(struct cstl_stack *s,uint64_t init_size,struct cstl_object_func *funcs);
struct cstl_object *cstl_stack_pop(struct cstl_stack *s);
struct cstl_object *cstl_stack_top(struct cstl_stack *s);
int cstl_stack_push(struct cstl_stack *s,struct cstl_object *obj);
int cstl_stack_expand(struct cstl_stack *s);
int cstl_stack_traverse(struct cstl_stack *s);
void cstl_stack_free(struct cstl_stack *s);
void cstl_stack_deinit(struct cstl_stack *s);

#endif
