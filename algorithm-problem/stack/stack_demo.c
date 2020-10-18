/*************************************************************************
  > File Name: stack_demo.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 日 10/18 11:38:42 2020
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../stack.h"
/* 设计一个栈获取最大和最小元素*/
void get_stack_min_and_max_element(int n)
{
  stack *s = stack_create(sizeof(int));
  assert(s!=NULL);
  int min,max;
  for(int i=0;i<n;i++)
  {
    int *data = (int *)stack_push_back(s);
    *data = rand()%1024;
    fprintf(stdout,"insert data=%d\n",*data);
    if(i==0) {
      min = max = *data;
    }else{
      if(*data<min) {
        min = *data;
      }
      if(*data>max){
        max = *data;
      }
    }
  }
  stack_destroy(s);
}
/**两个栈模拟一个队列**/
void stack_merge_to_queue(int n) 
{
  stack *s1 = stack_create(sizeof(int));
  assert(s1!=NULL);
  for(int i=0;i<n;i++)
  {
    int *data  = stack_push_back(s1);
    *data = rand()%1024;
    fprintf(stdout,"i=%d,data=%d\n",i,*data);

  }
  fprintf(stdout,"--------------queue--------------\n");
  stack *s2 = stack_create(sizeof(int));
  assert(s2!=NULL);
  for(int i=0;i<n;i++)
  {
    int *s1_data = stack_pop_back(s1);
    int *data = stack_push_back(s2);
    *data = *s1_data;
  }
  for(int i=0;i<n;i++)
  {
    int *s2_data = stack_pop_front(s2);
    fprintf(stdout,"i=%d,data=%d\n",i,*s2_data);
  }
}
/**转的翻转操作**/
static int reverse_stack(stack *s) {

  if(stack_len(s)>0) {
    int *data = (int *)stack_pop_back(s);
    int *data2=(int *)stack_push_front(s);
    *data2=*data;
    stack_release_elem(data);
  }
}



