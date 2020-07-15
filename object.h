/*************************************************************************
  > File Name: object.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:17:56 HKT
 ************************************************************************/

#ifndef _CSTL_OBJECT_H
#define _CSTL_OBJECT_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
typedef void (*cstl_object_data_free)(void *obj);
typedef void (*cstl_object_cb)(void *obj);
typedef enum cstl_object_type {
  CSTL_STRING_OBJECT = 0,
  CSTL_UINT64_OBJECT,
  CSTL_INT64_OBJECT,
  CSTL_UINT32_OBJECT,
  CSTL_INT32_OBJECT,
  CSTL_DOUBLE_OBJECT,
  CSTL_STRUCT_OBJECT
} cstl_object_type;

typedef struct cstl_object {
  union {
    int64_t i64;
    uint64_t u64;
    int32_t i32;
    uint32_t u32;
    double d64;
    void *ptr;
  } data;
  uint8_t type;
} cstl_object;

cstl_object *cstl_object_alloc(void *data, cstl_object_type obj_type);
void *cstl_object_data(cstl_object *obj);
void cstl_object_free(cstl_object *obj, cstl_object_data_free cb);
// true is nil
bool cstl_object_is_nil(cstl_object *obj);
#endif