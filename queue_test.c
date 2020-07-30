/*************************************************************************
  > File Name: queue_test.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 PM03:52:05 HKT
 ************************************************************************/

#include "queue.h"
#include "object.h"
#include <stdio.h>
#include <stdlib.h>
void cstl_object_out(cstl_object *obj)
{
  char *value = (char *)cstl_object_data(obj);
  fprintf(stdout, "output data:%s\n", value);
}
void test_string()
{
  cstl_object_func fn = {
      .data_free_func = free,
      .object_free_func = cstl_object_free,
      .object_process_func = cstl_object_out,
  };
  cstl_queue *q = cstl_queue_alloc(&fn);
  size_t n = 10;
  char **arrs = (char **)calloc(n, sizeof(char *));
  for (int i = 0; i < n; i++)
  {
    arrs[i] = calloc(64, sizeof(char));
    snprintf(arrs[i], 64, "hello,%d", rand() % 64 + 1);
    cstl_object *obj = cstl_object_alloc(arrs[i], CSTL_STRING_OBJECT);
    fprintf(stdout, "ret :%d, value:%s\n", cstl_queue_push(q, obj), arrs[i]);
  }
  fprintf(stdout, "-----------------------\n");
  cstl_queue_traverse(q);
  for (int i = 0; i < 5; i++)
  {
    cstl_object *obj = cstl_queue_pop(q);
    char *value = (char *)cstl_object_data(obj);
    fprintf(stdout, "first-pop value:%s\n", value);
  }
  for (int i = 0; i < 3; i++)
  {
    cstl_object *obj = cstl_queue_pop(q);
    char *value = (char *)cstl_object_data(obj);
    fprintf(stdout, "second-pop value:%s\n", value);
  }
  cstl_queue_free(q);
  free(arrs);
}

int main()
{
  test_string(cstl_object_out);
  return 0;
}
