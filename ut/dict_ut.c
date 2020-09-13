/*************************************************************************
  > File Name: dict_ut.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日  9/13 11:49:41 2020
 ************************************************************************/

#include "../dict.h"
#include <stdio.h>
#include <uuid/uuid.h>
int gen_uuid(char *str, size_t str_len)
{
  uuid_t uuid;
  uuid_generate(uuid);
  uuid_unparse(uuid, str);
}
void test_dict()
{
  size_t max = 20;
  dict *d = dict_create(max, NULL);
  size_t n = 10000;
  size_t i = 0;
  for (; i <= n; i++)
  {
    char buf[36] = {'\0'};
    gen_uuid((char *)&buf, 36);
    size_t *v = (size_t *)dict_add(d, (char *)&buf, sizeof(size_t));
    *v = n;
  }
  for (i = 0; i < max; i++)
  {
    if (d->member_count[i] > 0)
    {
      fprintf(stdout, "member_count[%d]=%d\n", i, d->member_count[i]);
    }
  }
  fprintf(stdout, "total:%d\n", d->count);
}
int main(void)
{
  test_dict();
  return 0;
}