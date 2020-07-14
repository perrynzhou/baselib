/*************************************************************************
  > File Name: stack.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Tuesday, July 14, 2020 PM05:28:38 HKT
 ************************************************************************/

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define CSTL_STACK_INIT_SIZE (8)
cstl_stack *cstl_stack_alloc(uint64_t init_size,cstl_object_data_free cb) {
  if (init_size < CSTL_STACK_INIT_SIZE) {
    init_size = CSTL_STACK_INIT_SIZE;
  }
  cstl_stack *s = (cstl_stack *)calloc(1,sizeof(cstl_stack));
  assert(s!=NULL);
  s->max_size = init_size;
  s->cur_size = 0;
  s->data = (cstl_object **)calloc(s->max_size, sizeof(cstl_object *));
  assert(s->data != NULL);
  s->cb = cb;
  return s;
}
cstl_object *cstl_stack_pop(cstl_stack *s) {
  cstl_object *obj = NULL;
  if (s != NULL && s->cur_size > 0) {
    obj = s->data[--s->cur_size];
  }
  return obj;
}
int cstl_stack_push(cstl_stack *s, cstl_object *obj) {
  if (s != NULL && obj != NULL && s->cur_size < s->max_size) {
    s->data[s->cur_size++] = obj;
    return 0;
  }
  return -1;
}
int cstl_stack_expand(cstl_stack *s) {
  if (s->cur_size < (s->max_size / 2)) {
    return 0;
  }
  cstl_object **data =
      (cstl_object **)calloc(s->max_size * (1.5), sizeof(cstl_object *));
  assert(data != NULL);
  s->max_size *= 1.5;
  for (size_t i = 0; i < s->cur_size; i++) {
    data[i] = s->data[i];
  }
  free(s->data);
  s->data = data;
  return 0;
}
void cstl_stack_free(cstl_stack *s) {
  if (s != NULL) {
    for (size_t i = 0; i < s->cur_size; i++) {
      if (s->cb != NULL) {
        cstl_object_free(s->data[i], s->cb);
      }
    }
    free(s->data);
    free(s);
    s = NULL;
  }
}
int cstl_stack_traverse(cstl_stack *s,cstl_object_cb  cb)
{

}
#ifdef CSTL_STACK_TEST
int main(void) {
  size_t n = 10;
  cstl_stack  *s = cstl_stack_alloc(n,NULL);
  cstl_object  *objs[10] = {NULL};
  for(int i=0;i<n;i++) {
    int data = rand()%30+i;
    objs[i] = cstl_object_alloc(&data,CSTL_UINT_OBJECT);
    cstl_stack_push(s,objs[i]);
    fprintf(stdout,"obj :%p,push data:%d\n",objs[i],data);
  }
  fprintf(stdout,"*********************\n");
  for(int i=0;i<n;i++){
    cstl_object *obj = cstl_stack_pop(s);
    int *data = (int *)cstl_object_data(obj);
    fprintf(stdout,"obj:%p,pop data:%d\n",obj,*data);
  }
  cstl_stack_free(s);
 }
#endif