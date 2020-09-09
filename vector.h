/*************************************************************************
    > File Name: vector.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Tuesday, September 08, 2020 PM04:59:37
 ************************************************************************/
#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <stdio.h>
#include <stdint.h>
typedef struct vector_t
{
  void *elem;
  uint32_t nelem;
  uint32_t nalloc;
  size_t size;
} vector;
typedef int (*vector_compare_fn)(const void *, const void *);
typedef int (*vector_each_fn)(void *, void *);

vector *vector_create(size_t size, size_t n);
uint32_t vector_size(vector *vec);
int vector_init(vector *vec, size_t size, size_t n);
uint32_t vector_index(vector *vec, void *elem);
void *vector_push(vector *vec);
void *vector_pop(vector *vec);
void *vector_get(vector *vec, uint32_t idx);
void vector_swap(vector *a, vector *b);
void vector_sort(vector *vec, vector_compare_fn compare);
int vector_each(vector *vec, vector_each_fn func, void *data);
void vector_deinit(vector *vec);
void vector_destroy(vector *vec);
#endif
