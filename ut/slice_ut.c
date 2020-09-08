/*************************************************************************
    > File Name: slice_ut.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Tuesday, September 08, 2020 AM11:19:31
 ************************************************************************/

#include "../slice.h"
#include <stdio.h>
#include <string.h>
void test_slice_create()
{
  const char *str = "abcdedfghyhd";
  size_t len = strlen(str);
  slice s = slice_create(str);
  fprintf(stdout, "str len=%ld,slice len:%ld,value:%s\n", len, slice_len(s), (char *)s);
}
int main(void)
{
  test_slice_create();
  return 0;
}