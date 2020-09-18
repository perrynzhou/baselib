/*************************************************************************
    > File Name: list_ut.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Thursday, September 10, 2020 PM03:57:55
 ************************************************************************/

#include "../list.h"
#include <stdio.h>
#include <stdlib.h>
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
void test_list_insert()
{
  int n = 10;
  /*
  list *lt = list_create(n);
  for (int i = 0; i < n; i++)
  {
    int *d = (int *)list_insert(lt,i);
    *d = i;
  }
  fprintf(stdout, "list len=%ld\n", list_len(lt));
  list_dump(lt, test_dump_cb);
  fprintf(stdout, "--------------end test_list_insert1--------------\n");

  list *lt2 = list_create(n);
  for (int i = 0; i < n; i++)
  {
    int *d = (int *)list_insert(lt2,0);
    *d = i;
  }
  fprintf(stdout, "list len=%ld\n", list_len(lt2));
  list_dump(lt2, test_dump_cb);
  fprintf(stdout, "--------------end test_list_insert2--------------\n");


  */
  list *lt3 = list_create(n);
  for (int i = 0; i < n; i++)
  {
    int pos = abs(rand() % n - 1);
    int *d = (int *)list_insert(lt3, pos);
    *d = i;
    fprintf(stdout, "i=%d,index=%d,insert pos:%d\n", i, list_len(lt3) - 1, pos);
    list_dump(lt3, test_dump_cb);
  }
  fprintf(stdout, "\nlist len=%ld\n", list_len(lt3));
  fprintf(stdout, "--------------end test_list_insert3--------------\n");
}
void test_list_remove()
{
  size_t n = 10;
  list *lt3 = list_create(n);
  for (int i = 0; i < n; i++)
  {
    int pos = abs(rand() % n - 2);
    int *d = (int *)list_insert(lt3, pos);
    *d = i;
    fprintf(stdout, "i=%d,index=%d,insert pos:%d\n", i, list_len(lt3) - 1, pos);
    list_dump(lt3, test_dump_cb);
  }
  fprintf(stdout, "\nlist len=%ld\n", list_len(lt3));
  fprintf(stdout, "--------------end test_list_insert3--------------\n");
  for (int i = 0; i < 3; i++)
  {
    int pos = abs(rand() % n - 2);
    void *data = list_remove(lt3, pos);
    list_release_elem(data);
    fprintf(stdout, "i=%d,remove pos:%d\n", i, pos);
  }
  list_dump(lt3, test_dump_cb);
  fprintf(stdout, "\nlist len=%ld\n", list_len(lt3));
  fprintf(stdout, "--------------end test_list_remove--------------\n");
}
int main(void)
{
  //test_list_push_back();
  // test_list_insert();
  test_list_remove();
  return 0;
}