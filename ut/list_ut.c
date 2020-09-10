/*************************************************************************
    > File Name: list_ut.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Thursday, September 10, 2020 PM03:57:55
 ************************************************************************/

#include "../list.h"
#include <stdio.h>
void test_dump_cb(void *data)
{
  int *d = (int *)data;
  fprintf(stdout, "data=%d\n", *d);
}
void test_list_push_back()
{
  int n = 4;
  list *lt = list_create(n);
  for (int i = 0; i < n; i++)
  {
    int *d = (int *)list_push_back(lt);
    *d = i;
  }
  fprintf(stdout, "list len=%ld\n", list_len(lt));
  list_dump(lt, test_dump_cb);
  fprintf(stdout, "----------------------------\n");
  list *lt2 = list_dup(lt);
  list_reverse(lt);
  list_dump(lt, test_dump_cb);
  fprintf(stdout, "----------------------------\n");
  for (int i = 0; i < n; i++)
  {
    list_dump(lt, test_dump_cb);
    void *data = list_pop_front(lt);
    list_release_elem(data);
    fprintf(stdout, "#############pop front list:%d#######\n", lt->nelem);
  }
  for (int i = 0; i < n; i++)
  {
    list_dump(lt2, test_dump_cb);
    void *data = list_pop_back(lt2);
    list_release_elem(data);
    fprintf(stdout, "#############pop back list:%d#######\n", lt->nelem);
  }
}
int main(void)
{
  test_list_push_back();
  return 0;
}