/*************************************************************************
    > File Name: slice.h
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Monday, September 07, 2020 AM10:26:10
 ************************************************************************/

#ifndef _SLICE_H
#define _SLICE_H
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
typedef char *slice;
int slice_init(slice *s,const char *str);
slice slice_create(const char *str);
size_t slice_len(const slice s);
slice slice_dup(const slice s);
void slice_destroy(slice s);
slice slice_append(slice s, const char *str);
slice slice_copy(slice s, const char *str);
slice slice_fmt(slice s, char const *fmt, ...);
slice slice_trim(slice s, const char *cset);
void slice_reset(slice s);
uint32_t slice_hash(slice s);
int slice_cmp(const slice s1, const slice s2);
int slice_atoi(slice s);
void slice_destroy(slice s);
void slice_tolower(slice s);
void slice_toupper(slice s);
void slice_deinit(slice s);
#endif
