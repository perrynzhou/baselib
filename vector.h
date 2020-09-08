/*************************************************************************
    > File Name: vector.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Tuesday, September 08, 2020 PM04:59:37
 ************************************************************************/

#ifndef _vector_H
#define _vector_H
/*
 * twemproxy - A fast and lightweight proxy for memcached protocol.
 * Copyright (C) 2011 Twitter, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <stdio.h>
#include <stdint.h>
typedef struct vector
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
void vector_swap(vector *vec, struct vector *b);
void vector_sort(vector *vec, vector_compare_fn compare);
int vector_each(vector *vec, vector_each_fn func, void *data);
void vector_deinit(vector *vec);
void vector_destroy(vector *vec);
#endif
