/*************************************************************************
  > File Name: queue_test.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 PM03:52:05 HKT
 ************************************************************************/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
void test_string(cstl_object_cb cb) {
  cstl_queue *q = cstl_queue_alloc((cstl_object_data_free)&free);
    size_t n = 10000;
  char **arrs = (char **)calloc(n,sizeof(char *));
  for (int i = 0; i < n; i++) {
    arrs[i] = calloc(64, sizeof(char));
    snprintf(arrs[i], 64, "hello,%d", rand() % 64 + 1);
    cstl_object *obj = cstl_object_alloc(arrs[i], CSTL_STRING_OBJECT);
    fprintf(stdout, "ret :%d, value:%s\n", cstl_queue_push(q, obj), arrs[i]);
  }
  fprintf(stdout, "-----------------------\n");
  cstl_queue_traverse(q, cb);
  for (int i = 0; i < 5000; i++) {
    cstl_object *obj = cstl_queue_pop(q);
    char *value = (char *)cstl_object_data(obj);
    fprintf(stdout, "first-pop value:%s\n", value);
  }
  for (int i = 0; i < 300; i++) {
    cstl_object *obj = cstl_queue_pop(q);
    char *value = (char *)cstl_object_data(obj);
    fprintf(stdout, "second-pop value:%s\n", value);
  }
  cstl_queue_free(q);
  free(arrs);
}
void cstl_object_out(void *d) {
  cstl_object *obj = (cstl_object *)d;
  char *value = (char *)cstl_object_data(obj);
  fprintf(stdout, "output data:%s\n", value);
}
int main() {
  test_string(cstl_object_out);
  return 0;
}
