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
cstl_object_func *cstl_object_func_alloc(cstl_object_release_data data_free_func,cstl_object_release object_free_func,cstl_object_process  object_process_func){
  if(data_free_func==NULL &&object_free_func==NULL &&object_process_func==NULL)
  {
    return NULL;
  }
  cstl_object_func *func = (cstl_object_func *)calloc(1,sizeof(cstl_object_func));
  if(func !=NULL) {
      func->data_free_func =data_free_func;
      func->object_free_func=object_free_func;
      func->object_process_func =object_process_func;
  }
  return func;

}
void cstl_object_func_free(cstl_object_func *func){
  if(func!=NULL){
    free(func);
    func = NULL;
  }
}
int cstl_object_init(cstl_object *obj, void *data, cstl_object_type obj_type) {
  if (obj == NULL || data == NULL || !(cstl_object_valid_type(obj_type))) {
    return -1;
  }
  obj->type = obj_type;
  switch (obj_type) {
  case CSTL_STRING_OBJECT:
    obj->data.ptr = strdup((char *)data);
    break;
  case CSTL_UINT64_OBJECT:
    obj->data.u64 = *((uint64_t *)data);
    break;
  case CSTL_INT64_OBJECT:
    obj->data.i64 = *((int64_t *)data);
    break;
  case CSTL_UINT32_OBJECT:
    obj->data.u32 = *((uint32_t *)data);
    break;
  case CSTL_INT32_OBJECT:
    obj->data.i32 = *((int32_t *)data);
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
static inline bool cstl_object_valid_type(cstl_object_type type) {
  if (type < CSTL_STRING_OBJECT || type > CSTL_STRUCT_OBJECT) {
    return false;
  }
  return true;
}
bool cstl_object_is_nil(cstl_object *obj) {
  if (obj == NULL) {
    return true;
  }
  return false;
}
cstl_object *cstl_object_alloc(void *data, cstl_object_type obj_type) {
  cstl_object *obj = (cstl_object *)calloc(1, sizeof(cstl_object));
  if (cstl_object_init(obj, data, obj_type) != 0) {
    free(obj);
    obj = NULL;
  }
  return obj;
}
void *cstl_object_data(cstl_object *obj) {
  void *data = NULL;
  switch (obj->type) {
  case CSTL_STRING_OBJECT:
    data = obj->data.ptr;
    break;
  case CSTL_UINT64_OBJECT:
    data = &obj->data.u64;
    break;
  case CSTL_INT64_OBJECT:
    data = &obj->data.i64;
    break;
  case CSTL_UINT32_OBJECT:
    data = &obj->data.u32;
    break;
  case CSTL_INT32_OBJECT:
    data = &obj->data.i32;
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
void cstl_object_deinit(cstl_object *obj, cstl_object_release_data cb) {
  if (!cstl_object_is_nil(obj)) {
    if (obj->type == CSTL_STRING_OBJECT || obj->type == CSTL_STRUCT_OBJECT) {
      if (cb != NULL) {
        cb(obj->data.ptr);
      }
    }
  }
}
void cstl_object_free(cstl_object *obj, cstl_object_release_data cb) {
  cstl_object_deinit(obj, cb);
  if (obj != NULL) {
    free(obj);
  }
}
