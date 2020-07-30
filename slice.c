/*************************************************************************
  > File Name: slice.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 PM04:24:08 HKT
 ************************************************************************/

#include "slice.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#define MAX_INT64_T_VALUE 9223372036854775807

size_t cstl_string_len(cstl_string *s) { return s->len; }
char *cstl_string_data(cstl_string *s) { return s->data; }

int cstl_string_init(cstl_string *s, const char *str) {
  if (s != NULL && str != NULL) {
    size_t len = strlen(str);
    s->len = len;
    s->data = strdup(str);
    return 0;
  }
  return -1;
}
int cstl_string_deinit(cstl_string *s) {
  if (s != NULL && s->len > 0) {
    free(s->data);
    s->len = 0;
    return 0;
  }
  return -1;
}
cstl_string *cstl_string_alloc(const char *str) {
 struct  cstl_string *s = NULL;
  if (str != NULL) {
    s = (cstl_string *)calloc(1, sizeof(cstl_string));
    if (cstl_string_init(s, str) != 0) {
      free(s);
      s = NULL;
    }
  }
  return s;
}
int cstl_string_strsep(cstl_string *s,char **save_results,size_t n,const char *delim)
{
  if(s==NULL||save_results==NULL||n<=0||delim==NULL) {
    return -1;
  }
  char *str = strdup(s->data);
  char *result = NULL;
  int i = 0;
  while((result=strsep(&str,delim))!=NULL)
  {
    save_results[i++]= strdup(result);
  }
  if(str!=NULL)
  {
    free(str);
  }
  return i;
}
int cstl_string_snprintf(cstl_string *s,  const char *format, ...) {

  va_list arg_origin;
  va_list arg_save;
  va_start(arg_origin, format);
  va_copy(arg_save, arg_origin);
  va_end(arg_origin);
  int size = vsnprintf(NULL, 0, format, arg_origin);
  
  s->data = (char *)calloc(size + 1, sizeof(char));
  vsnprintf(s->data, size + 1, format, arg_save);
  s->data[size] = '\0';
  s->len = size;
  va_end(arg_save);
  return size;
}
void cstl_string_free(cstl_string *s) {
  if (s != NULL) {
    if (s->len > 0) {
      cstl_string_deinit(s);
    }
    free(s);
  }
}
int64_t cstl_string_hextoi(cstl_string *s) {
  unsigned char c, ch;
  int64_t value, cutoff;
  char *line = cstl_string_data(s);
  size_t n = cstl_string_len(s);
  if (n == 0) {
    return -1;
  }
  cutoff = MAX_INT64_T_VALUE / 16;
  for (value = 0; n--; line++) {
    if (value > cutoff) {
      return -1;
    }
    ch = *line;
    if (ch >= '0' && ch <= '9') {
      value = value * 16 + (ch - '0');
      continue;
    }
    c = (unsigned char)(ch | 0x20);
    if (c >= 'a' && c <= 'f') {
      value = value * 16 + (c - 'a' + 10);
      continue;
    }
    return -1;
  }
  return value;
}
