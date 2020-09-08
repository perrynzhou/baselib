/*************************************************************************
  > File Name: vector.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Wednesday, July 22, 2020 PM05:19:27 HKT
 ************************************************************************/

#include "vector.h"
#define CSTL_VECTOR_MIN_SIZE (4)
cstl_vector *cstl_vector_alloc(size_t max, cstl_object_func *funcs)
{

  cstl_vector *vec = (cstl_vector *)calloc(1, sizeof(cstl_vector));
  if (cstl_vector_init(vec, max, funcs) != 0)
  {
    free(vec);
    vec = NULL;
  }
  return vec;
}
int cstl_vector_init(cstl_vector *v, size_t max, cstl_object_func *funcs)
{
  if (v == NULL)
  {
    return -1;
  }
  v->size = 0;
  v->max = (max < CSTL_VECTOR_MIN_SIZE) ? CSTL_VECTOR_MIN_SIZE : max;
  v->data = (cstl_object **)calloc(max, sizeof(cstl_vector *));
  if (v->data == NULL)
  {
    return -1;
  }
  if (funcs != NULL)
  {
    v->funcs->data_free_func = funcs->data_free_func;
    v->funcs->object_free_func = funcs->object_free_func;
    v->funcs->object_process_func = funcs->object_process_func;
  }
  return 0;
}
void cstl_vector_expand(cstl_vector *v)
{
  if (v != NULL)
  {
    cstl_object **old = v->data;
    size_t cur_size = v->size;
    v->data = (cstl_object **)calloc(v->max * 2, sizeof(cstl_object *));
    if (v->data != NULL)
    {
      for (size_t i = 0; i < cur_size; i++)
      {
        v->data[i] = old[i];
      }
      free(old);
    }
  }
}
int cstl_vector_push(cstl_vector *v, cstl_object *obj)
{
  if (v != NULL && obj != NULL)
  {
    if (v->size > (v->max / 2))
    {
      cstl_vector_expand(v);
    }
    v->data[v->size++] = obj;
    return 0;
  }
  return -1;
}
cstl_object *cstl_vector_pop(cstl_vector *v)
{
  if (v != NULL)
  {
    return v->data[--v->size];
  }
  return NULL;
}
int cstl_vector_insert(cstl_vector *v, size_t index, cstl_object *obj)
{
  if (v != NULL && obj != NULL)
  {
    if (index <= 0)
    {
      index = 0;
    }
    if (index >= v->max)
    {
      index = v->max - 1;
    }
    if (v->data[index] != NULL)
    {
      return -1;
    }
    v->data[index] = obj;
  }
  return 0;
  ;
}
cstl_object *cstl_vector_delete(cstl_vector *v, size_t index)
{
  cstl_object *obj = NULL;
  if (v != NULL && index >= 0 && index <= v->size)
  {
    obj = v->data[index];
    v->size--;
  }
  return obj;
}
void cstl_vector_deinit(cstl_vector *v)
{
}
void cstl_vector_free(cstl_vector *v)
{
}