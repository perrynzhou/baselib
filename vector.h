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
typedef struct cstl_vector_t
{
  size_t size;
  size_t max;
  cstl_object **data;
  cstl_object_func *funcs;
} cstl_vector;
cstl_vector *cstl_vector_alloc(size_t max,  cstl_object_func *funcs);
int cstl_vector_init(cstl_vector *v, size_t max,  cstl_object_func *funcs);
int cstl_vector_push(cstl_vector *v,  cstl_object *obj);
cstl_object *cstl_vector_pop(cstl_vector *v);
int cstl_vector_insert(cstl_vector *v, size_t index,  cstl_object *obj);
cstl_object *cstl_vector_delete(cstl_vector *v, size_t index);
void cstl_vector_deinit(cstl_vector *v);
void cstl_vector_free(cstl_vector *v);
#endif
