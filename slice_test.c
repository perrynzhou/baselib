/*************************************************************************
  > File Name: slice_test.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 PM05:14:42 HKT
 ************************************************************************/

#include "slice.h"
#include <stdio.h>
#include <stdlib.h>
int main(void) {
  cstl_string *s = cstl_string_alloc("1:2:3:4");
  char **results = (char **)calloc(1024, sizeof(char *));
  size_t n = 1024;
  int index = cstl_string_strsep(s, results, n, ":");
  for (int i = 0; i < index; i++) {
    fprintf(stdout, "value:%s\n", results[i]);
  }
  if (results != NULL) {
    free(results);
  }

  cstl_string s2;
  cstl_string_init(&s2, "hello,cstl_string");
  fprintf(stdout, "init value:%s\n", cstl_string_data(&s2));
  cstl_string_deinit(&s2);

  cstl_string s3;
  cstl_string_snprintf(&s3, "%s,%s", "hello", "0123456789");
  fprintf(stdout, "snprintf value:%s,len:%ld\n", cstl_string_data(&s3),
          cstl_string_len(&s3));
  cstl_string_deinit(&s3);
  return 0;
}
