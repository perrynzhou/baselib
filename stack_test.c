/*************************************************************************
  > File Name: stack_test.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 AM09:36:36 HKT
 ************************************************************************/

#include "object.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
void test_int(void) {
  size_t n = 100;

  cstl_stack *s = cstl_stack_alloc(n, NULL);
  cstl_object *objs[100] = {NULL};
  for (int i = 0; i < n; i++) {
    int data = rand() % 30 + i;
    objs[i] = cstl_object_alloc(&data, CSTL_INT32_OBJECT);
    cstl_stack_push(s, objs[i]);
    fprintf(stdout, "obj :%p,push data:%d\n", objs[i], data);
  }
  fprintf(stdout, "*********************\n");
  for (int i = 0; i < n; i++) {
    cstl_object *obj = cstl_stack_pop(s);
    int *data = (int *)cstl_object_data(obj);
    fprintf(stdout, "obj:%p,pop data:%d\n", obj, *data);
  }
  cstl_stack_free(s);
}
void test_string(void) {
  size_t n = 10;
  cstl_object_func fn = {
    .data_free_func = free,
    .object_free_func =cstl_object_free,
    .object_process_func = NULL,
  };
  cstl_stack *s = cstl_stack_alloc(n, &fn);
  cstl_object *objs[10] = {NULL};
  for (int i = 0; i < n; i++) {
    char *data = (char *)calloc(64, sizeof(char));
    snprintf(data, 64, "hello,%d", rand() % 64 + i);
    objs[i] = cstl_object_alloc(data, CSTL_STRING_OBJECT);
    cstl_stack_push(s, objs[i]);
    fprintf(stdout, "obj :%p,push data:%s\n", objs[i], data);
  }
  fprintf(stdout, "*********************\n");
  for (int i = 0; i < n / 2; i++) {
    cstl_object *obj = cstl_stack_pop(s);
    char *data = (char *)cstl_object_data(obj);
    fprintf(stdout, "obj:%p,pop data:%s\n", obj, data);
  }
  cstl_stack_free(s);
}
int main(void) {
  test_int();
  test_string();
  return 0;
}