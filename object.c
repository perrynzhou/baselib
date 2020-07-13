/*************************************************************************
  > File Name: object.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, July 13, 2020 PM02:18:05 HKT
 ************************************************************************/

#include "object.h"
#include <stdio.h>
cstl_object *cstl_object_alloc(cstl_object_type obj_type) {}
int cstl_object_init(cstl_object *obj, cstl_object_type obj_type) {}
const char *cstl_object_at_string(cstl_object *obj) {}
int64_t cstl_object_at_i64(cstl_object *obj) {}
uint64_t cstl_object_at_u64(cstl_object *obj) {}
double cstl_object_at_d64(cstl_object *obj) {}
void *cstl_object_at_struct(cstl_object *obj) {}
int cstl_object_deinit(cstl_object *obj) {}
void cstl_object_free(cstl_object *obj) {}