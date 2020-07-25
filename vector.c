/*************************************************************************
  > File Name: vector.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Wednesday, July 22, 2020 PM05:19:27 HKT
 ************************************************************************/

#include "vector.h"
#define CSTL_VECTOR_MIN_SIZE   (4)
struct cstl_vector *cstl_vector_alloc(size_t max,struct cstl_object_func *funcs){
 
  struct cstl_vector *vec = (struct cstl_vector *)calloc(1,sizeof(struct cstl_vector));
  if(cstl_vector_init(vec,max,funcs)!=0) {
      free(vec);
      vec = NULL;
  }
  return vec;
}
int cstl_vector_init(struct cstl_vector *v,size_t max,struct cstl_object_func *funcs){
   if(v==NULL) {
     return -1;
   }
   v->size =0;
   v->max=(max <CSTL_VECTOR_MIN_SIZE)?CSTL_VECTOR_MIN_SIZE:max;
   v->data = (struct cstl_object **)calloc(max,sizeof(struct cstl_vector *));
   if(v->data==NULL) {
     return -1;
   }
   if(funcs!=NULL) {
     v->funcs->data_free_func=funcs->data_free_func;
     v->funcs->object_free_func=funcs->object_free_func;
     v->funcs->object_process_func=funcs->object_process_func;
   }
   return 0;
}
void cstl_vector_expand(struct cstl_vector *v) {

}
int cstl_vector_push(struct cstl_vector *v,struct cstl_object *obj){
  if(v!=NULL && obj!=NULL)
  {
    if(v->size>(v->max/2)){
      cstl_vector_expand(v);
    }
    v->data[v->size++]= obj;
    return 0;
  }
  return -1;
}
struct cstl_obejct *cstl_vector_pop(struct cstl_vector *v){

}
struct cstl_obejct * cstl_vector_insert(struct cstl_vector *v,size_t index,struct cstl_object *obj){

}
struct cstl_obejct * cstl_vector_delete(struct cstl_vector *v,size_t index){

}
void cstl_vector_deinit(struct cstl_vector *v){

}
void cstl_vector_free(struct cstl_vector *v){

}