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
 struct cstl_string {
  size_t len;
  char *data;
} ;
struct cstl_string *cstl_string_alloc(const char *str);
int cstl_string_init(struct cstl_string *s, const char *str);
int cstl_string_snprintf(struct cstl_string *s, const char *format, ...);
int64_t cstl_string_hextoi(struct cstl_string *s);
size_t cstl_string_len(struct cstl_string *s);
int cstl_string_strsep(struct cstl_string *s, char **save_results, size_t n,
                       const char *delim);
char *cstl_string_data(struct cstl_string *s);
int cstl_string_deinit(struct cstl_string *s);
void cstl_string_free(struct cstl_string *s);
#endif
