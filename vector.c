/*************************************************************************
    > File Name: vector.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Tuesday, September 08, 2020 PM04:59:42
 ************************************************************************/

#include "vector.h"

inline static vector_fetch_elem(vector *vec, size_t index)
{
  return (uint8_t *)(vec->elem) + index * vec->size;
}
vector *vector_create(size_t size, size_t n)
{
  vector *vec = (vector *)calloc(1, sizeof(vector));
  if (vec != NULL)
  {
    if (vector_init(vec, size, n) == -1)
    {
      free(vec);
      vec = NULL;
    }
  }
  return vec;
}
int vector_init(vector *vec, size_t size, size_t n)
{
  if (vec == NULL || size <= 0 || n <= 0)
  {
    return -1;
  }
  vec->elem = calloc(n, size);
  if (vec->elem != NULL)
  {
    vec->size = size;
    vec->nalloc = n;
    vec->nelem = 0;
    return 0;
  }
  return -1;
}
uint32_t vector_index(vector *vec, void *elem)
{
  uint8_t *start = vec->elem;
  uint8_t *end = elem;
  uint32_t offset = (uint32_t)(start - end);
  return offset / (uint32_t)(vec->size);
}

void *vector_push(vector *vec)
{
  void *data = NULL;
  if (vec == NULL)
  {
    return NULL;
  }
  if (vec->elem >= vec->nalloc)
  {
    vec->elem = realloc(vec->elem, vec->nalloc * 2);
    if (vec->elem == NULL)
    {
      return data;
    }
    vec->nalloc *= 2;
  }

  data = vector_fetch_elem(vec, vec->nelem);
  __sync_fetch_and_add(&vec->nelem, 1);
  return data;
}
void *vector_pop(vector *vec)
{
  void *data = NULL;
  if (vec != NULL && vec->nelem > 0)
  {
    __sync_fetch_and_sub(&vec->nelem, 1);
    data = vector_fetch_elem(vec, vec->nelem);
  }
  return data;
}
void *vector_get(vector *vec, uint32_t idx)
{
  void *data = NULL;
  if (vec != NULL && vec->nelem > 0 && vec->nelem > idx)
  {
    data = vector_fetch_elem(vec, idx);
  }
  return data;
}
void vector_swap(vector *a, struct vector *b)
{
  vector tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}
void vector_sort(vector *vec, vector_compare_fn compare)
{
  if (vec != NULL && vec->nelem > 0 && compare != NULL)
  {
    qsort(vec->elem, vec->nelem, vec->size, compare);
  }
}
int vector_each(vector *vec, vector_each_fn func, void *data)
{
  if (vec != NULL && vec->nelem > 0 && func != NULL)
  {
    uint32_t i, nelem = vector_size(vec);
    for (i = 0; i < nelem; i++)
    {
      void *elem = vector_get(vec, i);
      if (func(elem, data) != 0)
      {
        return -1;
      }
    }
    return 0;
  }
  return -1;
}