/*************************************************************************
  > File Name: vector.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 22, 2020 PM03:15:59 HKT
 ************************************************************************/

#ifndef _VECTOR_H
#define _VECTOR_H
#include "object.h"
#include <stdio.h>
struct cstl_vector {
  size_t size;
  size_t max;
  struct cstl_object **data;
  struct cstl_object_func *funcs;
};
struct cstl_vector *cstl_vector_alloc(size_t max,struct  cstl_object_func *funcs);
int cstl_vector_init(struct cstl_vector *v,size_t max,struct cstl_object_func *funcs);
int cstl_vector_push(struct cstl_vector *v,struct cstl_object *obj);
struct cstl_obejct *cstl_vector_pop(struct cstl_vector *v);
struct cstl_obejct * cstl_vector_insert(struct cstl_vector *v,size_t index,struct cstl_object *obj);
struct cstl_obejct * cstl_vector_delete(struct cstl_vector *v,size_t index);
void cstl_vector_deinit(struct cstl_vector *v);
void cstl_vector_free(struct cstl_vector *v);
#endif
