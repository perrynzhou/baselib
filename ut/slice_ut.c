/*************************************************************************
    > File Name: slice_ut.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Tuesday, September 08, 2020 AM11:19:31
 ************************************************************************/

#include "../inc/slice.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
void test_slice_create()
{
  const char *str = "abcdedfghyhd";
  size_t len = strlen(str);
  slice s = slice_create(str);
  fprintf(stdout, "str len=%ld,slice len:%ld,value:%s\n", len, slice_len(s), (char *)s);

}
void test_slice_init()
{
  const char *str = "abcdedfghyhd";
  size_t len = strlen(str);
  slice s;
  int ret = slice_init(&s,str);
  fprintf(stdout, "str len=%ld,slice len:%ld,value:%s\n", len, slice_len(s), (char *)s);
  slice_deinit(&s);
}
int main(void)
{
  test_slice_create();
  fprintf(stdout,"----------\n");
  test_slice_init();
  return 0;
}