/*************************************************************************
  > File Name: dict_ut.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  9/13 11:49:41 2020
 ************************************************************************/

#include "../dict.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uuid/uuid.h>
typedef struct
{
  int v;
  char *d;
} test_s;
int gen_uuid(char *str, size_t str_len)
{
  uuid_t uuid;
  uuid_generate(uuid);
  uuid_unparse(uuid, str);
}

int test_dict_dump_cb(char *key, void *data)
{
  test_s *ts = (test_s *)data;
  fprintf(stdout, "%s:%d\n", key, ts->v);
  return 0;
}
void test_dict_free(void *arg)
{
  test_s *ts = (test_s *)arg;
  fprintf(stdout, "free  %s data\n", ts->d);
  free(ts->d);
}
void test_dict()
{
  size_t max = 2;
  dict *d = dict_create(max, NULL);
  size_t n = 5;
  size_t i = 0;
  char *keys[n];
  for (; i < n; i++)
  {
    char buf[36] = {'\0'};
    gen_uuid((char *)&buf, 36);
    keys[i] = strdup(buf);
    test_s *ts = dict_add(d, (char *)&buf, sizeof(test_s));
    memcpy((void *)&ts->v, (void *)&i, sizeof(int));
    ts->d = keys[i];
    //memcpy((void *)v, (void *)&i, sizeof(int));
  }
  dict_dump(d, test_dict_dump_cb);
  fprintf(stdout, "origin total:%d\n", d->count);

  for (i = 3; i < n; i++)
  {
    dict_del(d, keys[i], test_dict_free);
  }
  fprintf(stdout, "after delete %d total :%d\n", n - 3, d->count);
  dict_dump(d, test_dict_dump_cb);
  for (i = 0; i < max; i++)
  {
    if (d->member_count[i] > 0)
    {
      fprintf(stdout, "member_count[%d]=%d\n", i, d->member_count[i]);
    }
  }

  dict_destroy(d);
}
int main(void)
{
  test_dict();
  return 0;
}