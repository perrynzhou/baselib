/*************************************************************************
  > File Name: object.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:18:05 HKT
 ************************************************************************/

#include "object.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int cstl_object_deinit(cstl_object *obj, cstl_object_data_free cb);
static int cstl_object_init(cstl_object *obj, void *data,
                            cstl_object_type obj_type);
static inline bool cstl_object_valid_type(cstl_object_type type) {
  if (type < CSTL_STRING_OBJECT || type > CSTL_STRUCT_OBJECT) {
    return false;
  }
  return true;
}
bool cstl_object_nil(cstl_object *obj) {
  if (obj == NULL) {
    return false;
  }
  return true;
}
cstl_object *cstl_object_alloc(void *data, cstl_object_type obj_type) {
  cstl_object *obj = (cstl_object *)calloc(1, sizeof(cstl_object));
  assert(obj != NULL);
  if (cstl_object_init(obj, data, obj_type) != 0) {
    free(obj);
    obj = NULL;
  }
  return obj;
}
static int cstl_object_init(cstl_object *obj, void *data,
                            cstl_object_type obj_type) {
  if (!cstl_object_nil(obj) || !(cstl_object_valid_type(obj_type))) {
    return -1;
  }
  size_t len = 0;
  obj->type = obj->type;
  switch (obj_type) {
  case CSTL_STRING_OBJECT:
    obj->data.ptr = strdup((char *)data);
    break;
  case CSTL_UINT_OBJECT:
    obj->data.u64 = *((uint64_t *)data);
    break;
  case CSTL_INT_OBJECT:
    obj->data.i64 = *((int64_t *)data);
    break;
  case CSTL_DOUBLE_OBJECT:
    obj->data.d64 = *((double *)data);
    break;
  case CSTL_STRUCT_OBJECT:
    obj->data.ptr = data;
    break;
  default:
    break;
  }
  return 0;
}
void *cstl_object_data(cstl_object *obj) {
  void *data = NULL;
  switch (obj->type) {
  case CSTL_STRING_OBJECT:
    data = obj->data.ptr;
    break;
  case CSTL_UINT_OBJECT:
    data = &obj->data.u64;
    break;
  case CSTL_INT_OBJECT:
    data = &obj->data.i64;
    break;
  case CSTL_DOUBLE_OBJECT:
    data = &obj->data.d64;
    break;
  case CSTL_STRUCT_OBJECT:
    data = &obj->data.ptr;
    break;
  default:
    break;
  }
  return data;
}

static int cstl_object_deinit(cstl_object *obj, cstl_object_data_free cb) {
  if (!cstl_object_nil(obj)) {
    if (obj->type == CSTL_STRING_OBJECT && cb == NULL) {
      return -1;
    }
    if (obj->type == CSTL_STRING_OBJECT && cb != NULL) {
      cb(obj->data.ptr);
      return 0;
    }
    if (obj->type == CSTL_STRING_OBJECT) {
      free(obj->data.ptr);
      obj->data.ptr = NULL;
      return 0;
    }
  }
  return -1;
}
int cstl_object_free(cstl_object *obj, cstl_object_data_free cb) {
  if (!cstl_object_nil(obj)) {
    if (cstl_object_deinit(obj, cb) != 0) {
      return -1;
    }
    free(obj);
    return 0;
  }
  return -1;
}
