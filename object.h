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
typedef void (*cstl_object_release_data)(void *data);
typedef void (*cstl_object_release)(cstl_object *obj,cstl_object_release_data cb);
typedef void (*cstl_object_process)(cstl_object *obj);
typedef struct cstl_object_func {
  // how to release data in cstl_object
  cstl_object_release_data data_free_func;
  // which release method for cstl_object,deinit or free
  cstl_object_release object_free_func;
  // method for process cstl_object
  cstl_object_process object_process_func;
} cstl_object_func;

cstl_object_func *
cstl_object_func_alloc(cstl_object_release_data data_free_func,
                       cstl_object_release object_free_func,
                       cstl_object_process object_process_func);
void cstl_object_func_free(cstl_object_func *func);
cstl_object *cstl_object_alloc(void *data, cstl_object_type obj_type);
int cstl_objeect_init(cstl_object *obj, void *data, cstl_object_type obj_type);
void *cstl_object_data(cstl_object *obj);
void cstl_object_deinit(cstl_object *obj, cstl_object_release_data cb);
void cstl_object_free(cstl_object *obj, cstl_object_release_data cb);
// true is nil
bool cstl_object_is_nil(cstl_object *obj);
#endif