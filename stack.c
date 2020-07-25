/*************************************************************************
  > File Name: stack.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Tuesday, July 14, 2020 PM05:28:38 HKT
 ************************************************************************/

#include "stack.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CSTL_STACK_INIT_SIZE (8)
int cstl_stack_init(struct cstl_stack *s, uint64_t init_size,
                    struct cstl_object_func *funcs) {

  if (s == NULL) {
    return -1;
  }
  memset(s, 0, sizeof(struct cstl_stack));
  if (init_size < CSTL_STACK_INIT_SIZE) {
    init_size = CSTL_STACK_INIT_SIZE;
  }
  s->max_size = init_size;
  s->cur_size = 0;
  s->data =
      (struct cstl_object **)calloc(s->max_size, sizeof(struct cstl_object *));
  assert(s->data != NULL);
  if (funcs != NULL) {
    s->funcs =
        cstl_object_func_alloc(funcs->data_free_func, funcs->object_free_func,
                               funcs->object_process_func);
    if (s->funcs == NULL) {
      free(s->data);
      s->data = NULL;
      return -1;
    }
  }
  return 0;
}
struct cstl_stack *cstl_stack_alloc(uint64_t init_size,
                                    struct cstl_object_func *funcs) {

  struct cstl_stack *s =
      (struct cstl_stack *)calloc(1, sizeof(struct cstl_stack));
  if (cstl_stack_init(s, init_size, funcs) != 0) {
    if (s != NULL && s->data != NULL) {
      free(s);
      s = NULL;
    }
  }
  return s;
}
struct cstl_object *cstl_stack_pop(struct cstl_stack *s) {
  struct cstl_object *obj = NULL;
  if (s != NULL && s->cur_size > 0) {
    obj = s->data[--s->cur_size];
  }
  return obj;
}
int cstl_stack_push(struct cstl_stack *s, struct cstl_object *obj) {
  if (s != NULL && obj != NULL && s->cur_size < s->max_size) {
    s->data[s->cur_size++] = obj;
    return 0;
  }
  return -1;
}
int cstl_stack_expand(struct cstl_stack *s) {
  if (s->cur_size < (s->max_size / 2)) {
    return 0;
  }
  struct cstl_object **data = (struct cstl_object **)calloc(
      s->max_size * (1.5), sizeof(struct cstl_object *));
  assert(data != NULL);
  s->max_size *= 1.5;
  for (size_t i = 0; i < s->cur_size; i++) {
    data[i] = s->data[i];
  }
  free(s->data);
  s->data = data;
  return 0;
}
void cstl_stack_deinit(struct cstl_stack *s) {
  if (s != NULL) {
    for (size_t i = 0; i < s->cur_size; i++) {
      struct cstl_object *obj = s->data[i];
      if (s->funcs != NULL) {
        if (s->funcs->object_free_func != NULL) {
          s->funcs->object_free_func(obj, s->funcs->data_free_func);
        }
      }
    }
    if (s->funcs != NULL) {
      cstl_object_func_free(s->funcs);
    }
    free(s->data);
    s = NULL;
  }
}
void cstl_stack_free(struct cstl_stack *s) {
  cstl_stack_deinit(s);
  if (s != NULL) {
    free(s);
    s = NULL;
  }
}
int cstl_stack_traverse(struct cstl_stack *s) {}
struct cstl_object *cstl_stack_top(struct cstl_stack *s) {
  struct cstl_object *obj = NULL;
  if (s != NULL && s->cur_size > 0) {
    obj = s->data[s->cur_size - 1];
  }
  return obj;
}
