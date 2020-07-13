/*************************************************************************
  > File Name: object.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:17:56 HKT
 ************************************************************************/

#ifndef _CSTL_OBJECT_H
#define _CSTL_OBJECT_H
#include <stdint.h>
#include <stdio.h>
typedef enum cstl_object_type {
  CSTL_STRING_OBJECT = 0,
  CSTL_INT_OBJECT,
  CSTL_DOUBLE_OBJECT,
  CSTL_STRUCT_OBJECT
} cstl_object_type;

typedef struct cstl_object {
  union {
    char buf[8];
    int64_t i64;
    double d64;
    uint64_t u64;
    void *ptr;
  } data;
  size_t len;
  uint8_t type;
} cstl_object;

cstl_object *cstl_object_alloc(cstl_object_type obj_type);
int cstl_object_init(cstl_object *obj, cstl_object_type obj_type);
const char *cstl_object_at_string(cstl_object *obj);
int64_t cstl_object_at_i64(cstl_object *obj);
uint64_t cstl_object_at_u64(cstl_object *obj);
double cstl_object_at_d64(cstl_object *obj);
void *cstl_object_at_struct(cstl_object *obj);
int cstl_object_deinit(cstl_object *obj);
void cstl_object_free(cstl_object *obj);
int cstl_object_compare(cstl_object *src, cstl_object *dst);
void cstl_object_memcpy(cstl_object *src, cstl_object *dst);
#endif
