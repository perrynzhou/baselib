/*************************************************************************
    > File Name: slice.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, September 07, 2020 AM10:26:10
 ************************************************************************/

#ifndef _slice_H
#define _slice_H
#include <stdarg.h>
#include <stdio.h>
typedef char *slice;
slice slice_create(const char *str);
size_t slice_len(const slice s);
slice slice_dup(const slice s);
void slice_destroy(slice s);
slice slice_append(slice s, const char *str);
slice slice_copy(slice s, const char *str);
slice slice_fmt(slice s, char const *fmt, ...);
slice slice_trim(slice s, const char *cset);
void slice_reset(slice s);
int slice_cmp(const slice s1, const slice s2);
void slice_tolower(slice s);
void slice_toupper(slice s);
#endif
