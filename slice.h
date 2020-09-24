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

//convert to int
int slice_atoi(unsigned char *line, size_t n);
//convert to xxx
ngx_int_t ngx_atofp(u_char *line, size_t n, size_t point);
//convert to ssize_t
ssize_t ngx_atosz(u_char *line, size_t n);
//convert to off_t
off_t ngx_atoof(u_char *line, size_t n);
//convert to timt_t
time_t ngx_atotm(u_char *line, size_t n);
//convert to int
ngx_int_t ngx_hextoi(u_char *line, size_t n);

u_char *ngx_hex_dump(u_char *dst, u_char *src, size_t len);


#define ngx_base64_encoded_length(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_length(len)  (((len + 3) / 4) * 3)

void ngx_encode_base64(ngx_str_t *dst, ngx_str_t *src);
void ngx_encode_base64url(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64(ngx_str_t *dst, ngx_str_t *src);
ngx_int_t ngx_decode_base64url(ngx_str_t *dst, ngx_str_t *src);
#endif
