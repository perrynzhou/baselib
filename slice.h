/*************************************************************************
  > File Name: slice.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Wednesday, July 15, 2020 PM04:24:04 HKT
 ************************************************************************/

#ifndef _SLICE_H
#define _SLICE_H
#include <stdint.h>
#include <stdio.h>
typedef  struct cstl_string_t {
  size_t len;
  char *data;
} cstl_string;
cstl_string *cstl_string_alloc(const char *str);
int cstl_string_init(cstl_string *s, const char *str);
int cstl_string_snprintf(cstl_string *s, const char *format, ...);
int64_t cstl_string_hextoi(cstl_string *s);
size_t cstl_string_len(cstl_string *s);
int cstl_string_strsep(cstl_string *s, char **save_results, size_t n,
                       const char *delim);
char *cstl_string_data(cstl_string *s);
int cstl_string_deinit(cstl_string *s);
void cstl_string_free(cstl_string *s);
#endif
