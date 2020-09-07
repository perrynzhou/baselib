/*************************************************************************
    > File Name: slice.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Monday, September 07, 2020 AM10:26:15
 ************************************************************************/
#include "slice.h"
#include <stdio.h>
#include <stdint.h>
#define SLICE_TYPE_5 0
#define SLICE_TYPE_8 1
#define SLICE_TYPE_16 2
#define SLICE_TYPE_32 3
#define SLICE_TYPE_64 4
#define SLICE_TYPE_MASK 7
#define SLICE_TYPE_BITS 3
#define SLICE_HDR_VAR(T, s) struct slice_hdr##T *sh = (void *)((s) - (sizeof(struct slice_hdr##T)))
#define SLICE_HDR(T, s) ((struct slice_hdr##T *)((s) - (sizeof(struct slice_hdr##T))))
#define SLICE_MAX_PREALLOC (1024 * 1024)
#define SLICE_TYPE_5_LEN(f) ((f) >> SLICE_TYPE_BITS)
struct __attribute__((__packed__)) slice_hdr5
{
  unsigned char flags;
  char buf[];
};
struct __attribute__((__packed__)) slice_hdr8
{
  uint8_t len;
  uint8_t alloc;
  unsigned char flags;
  char buf[];
};
struct __attribute__((__packed__)) slice_hdr16
{
  uint16_t len;
  uint16_t alloc;
  unsigned char flags;
  char buf[];
};
struct __attribute__((__packed__)) slice_hdr32
{
  uint32_t len;
  uint32_t alloc;
  unsigned char flags;
  char buf[];
};
struct __attribute__((__packed__)) slice_hdr64
{
  uint64_t len;
  uint64_t alloc;
  unsigned char flags;
  char buf[];
};

static inline int slice_hdr_size(char type)
{
  switch (type & SLICE_TYPE_MASK)
  {
  case SLICE_TYPE_5:
    return sizeof(struct slice_hdr5);
  case SLICE_TYPE_8:
    return sizeof(struct slice_hdr8);
  case SLICE_TYPE_16:
    return sizeof(struct slice_hdr16);
  case SLICE_TYPE_32:
    return sizeof(struct slice_hdr32);
  case SLICE_TYPE_64:
    return sizeof(struct slice_hdr64);
  }
  return 0;
}
static inline char slice_req_type(size_t string_size)
{
  if (string_size < 1 << 5)
    return SLICE_TYPE_5;
  if (string_size < 1 << 8)
    return SLICE_TYPE_8;
  if (string_size < 1 << 16)
    return SLICE_TYPE_16;
#if (LONG_MAX == LLONG_MAX)
  if (string_size < 1ll << 32)
    return SLICE_TYPE_32;
  return SLICE_TYPE_64;
#else
  return SLICE_TYPE_32;
#endif
}
size_t slice_len(const slice s)
{
  unsigned char flags = s[-1];
  switch (flags & SLICE_TYPE_MASK)
  {
  case SLICE_TYPE_5:
    return SLICE_TYPE_5_LEN(flags);
  case SLICE_TYPE_8:
    return SLICE_HDR(8, s)->len;
  case SLICE_TYPE_16:
    return SLICE_HDR(16, s)->len;
  case SLICE_TYPE_32:
    return SLICE_HDR(32, s)->len;
  case SLICE_TYPE_64:
    return SLICE_HDR(64, s)->len;
  }
  return 0;
}
static inline void slice_set_len(slice s, size_t newlen)
{
  unsigned char flags = s[-1];
  switch (flags & SLICE_TYPE_MASK)
  {
  case SLICE_TYPE_5:
  {
    unsigned char *fp = ((unsigned char *)s) - 1;
    *fp = SLICE_TYPE_5 | (newlen << SLICE_TYPE_BITS);
  }
  break;
  case SLICE_TYPE_8:
    SLICE_HDR(8, s)->len = newlen;
    break;
  case SLICE_TYPE_16:
    SLICE_HDR(16, s)->len = newlen;
    break;
  case SLICE_TYPE_32:
    SLICE_HDR(32, s)->len = newlen;
    break;
  case SLICE_TYPE_64:
    SLICE_HDR(64, s)->len = newlen;
    break;
  }
}
static inline void slice_set_alloc(slice s, size_t newlen)
{
  unsigned char flags = s[-1];
  switch (flags & SLICE_TYPE_MASK)
  {
  case SLICE_TYPE_5:
    /* Nothing to do, this type has no total allocation info. */
    break;
  case SLICE_TYPE_8:
    SLICE_HDR(8, s)->alloc = newlen;
    break;
  case SLICE_TYPE_16:
    SLICE_HDR(16, s)->alloc = newlen;
    break;
  case SLICE_TYPE_32:
    SLICE_HDR(32, s)->alloc = newlen;
    break;
  case SLICE_TYPE_64:
    SLICE_HDR(64, s)->alloc = newlen;
    break;
  }
}
/* slice_alloc() = slicea_vail() + slice_len() */
static inline size_t slice_alloc(const slice s)
{
  unsigned char flags = s[-1];
  switch (flags & SLICE_TYPE_MASK)
  {
  case SLICE_TYPE_5:
    return SLICE_TYPE_5_LEN(flags);
  case SLICE_TYPE_8:
    return SLICE_HDR(8, s)->alloc;
  case SLICE_TYPE_16:
    return SLICE_HDR(16, s)->alloc;
  case SLICE_TYPE_32:
    return SLICE_HDR(32, s)->alloc;
  case SLICE_TYPE_64:
    return SLICE_HDR(64, s)->alloc;
  }
  return 0;
}

static inline size_t slice_avail(const slice s)
{
  unsigned char flags = s[-1];
  switch (flags & SLICE_TYPE_MASK)
  {
  case SLICE_TYPE_5:
  {
    return 0;
  }
  case SLICE_TYPE_8:
  {
    SLICE_HDR_VAR(8, s);
    return sh->alloc - sh->len;
  }
  case SLICE_TYPE_16:
  {
    SLICE_HDR_VAR(16, s);
    return sh->alloc - sh->len;
  }
  case SLICE_TYPE_32:
  {
    SLICE_HDR_VAR(32, s);
    return sh->alloc - sh->len;
  }
  case SLICE_TYPE_64:
  {
    SLICE_HDR_VAR(64, s);
    return sh->alloc - sh->len;
  }
  }
  return 0;
}
static slice slice_alloc_mem(size_t initlen)
{
  if (initlen == 0)
  {
    return NULL;
  }
  void *sh;
  slice s;
  char type = slice_req_type(initlen);
  if (type == SLICE_TYPE_5 && initlen == 0)
    type = SLICE_TYPE_8;
  int hdrlen = slice_hdr_size(type);
  unsigned char *fp;

  sh = malloc(hdrlen + initlen + 1);
  if (sh == NULL)
  {
    return NULL;
  }
  memset(sh, 0, hdrlen + initlen + 1);
  s = (char *)sh + hdrlen;
  fp = ((unsigned char *)s) - 1;
  switch (type)
  {
  case SLICE_TYPE_5:
  {
    *fp = type | (initlen << SLICE_TYPE_BITS);
    break;
  }
  case SLICE_TYPE_8:
  {
    SLICE_HDR_VAR(8, s);
    sh->len = initlen;
    sh->alloc = initlen;
    *fp = type;
    break;
  }
  case SLICE_TYPE_16:
  {
    SLICE_HDR_VAR(16, s);
    sh->len = initlen;
    sh->alloc = initlen;
    *fp = type;
    break;
  }
  case SLICE_TYPE_32:
  {
    SLICE_HDR_VAR(32, s);
    sh->len = initlen;
    sh->alloc = initlen;
    *fp = type;
    break;
  }
  case SLICE_TYPE_64:
  {
    SLICE_HDR_VAR(64, s);
    sh->len = initlen;
    sh->alloc = initlen;
    *fp = type;
    break;
  }
  }
  s[initlen] = '\0';
  return s;
}
static slice slice_make_room_for(slice s, size_t addlen)
{
  void *sh, *newsh;
  size_t avail = slice_avail(s);
  size_t len, newlen;
  char type, oldtype = s[-1] & SLICE_TYPE_MASK;
  int hdrlen;

  if (avail >= addlen)
  {
    return s;
  }

  len = slice_len(s);
  sh = (char *)s - slice_hdr_size(oldtype);
  newlen = (len + addlen);
  if (newlen < SLICE_MAX_PREALLOC)
  {
    newlen *= 2;
  }
  else
  {
    newlen += SLICE_MAX_PREALLOC;
  }
  type = slice_req_type(newlen);
  if (type == SLICE_TYPE_5)
  {
    type = SLICE_TYPE_8;
  }

  hdrlen = slice_hdr_size(type);
  //扩容前的slicehdr类型和扩容后的类型一致，则在原来基础上进行realloc
  if (oldtype == type)
  {
    newsh = realloc(sh, hdrlen + newlen + 1);
    if (newsh == NULL)
    {
      return NULL;
    }
    s = (char *)newsh + hdrlen;
  }
  else
  {
    newsh = malloc(hdrlen + newlen + 1);
    if (newsh == NULL)
    {
      return NULL;
    }
    memcpy((char *)newsh + hdrlen, s, len + 1);
    free(sh);
    s = (char *)newsh + hdrlen;
    s[-1] = type;
    slice_set_len(s, len);
  }
  slice_set_alloc(s, newlen);
  return s;
}
slice slice_dup(const slice s)
{
  return slice_create(s);
}
slice slice_copy(slice s, const char *str)
{
  if (str == NULL)
  {
    return s;
  }
  size_t len = strlen(str);
  if (slice_alloc(s) < len)
  {
    s = slice_make_room_for(s, len - slicelen(s));
    if (s == NULL)
    {
      return NULL;
    }
  }
  memcpy(s, str, len);
  s[len] = '\0';
  slice_set_len(s, len);
  return s;
}

slice slice_append(slice s, const char *str)
{
  if (str == NULL)
  {
    return s;
  }
  size_t len = strlen(str);
  size_t curlen = slice_len(s);

  s = slice_make_room_for(s, len);
  if (s == NULL)
  {
    return NULL;
  }
  memcpy(s + curlen, str, len);
  slice_set_len(s, curlen + len);
  s[curlen + len] = '\0';
  return s;
}
slice slice_create(const char *str)
{
  if (str == NULL)
  {
    return NULL;
  }
  size_t initlen = strlen(str);
  slice s = slice_alloc_mem(initlen);
  if (s != NULL)
  {
    memcpy(s, str, initlen);
  }
  return s;
}
slice slice_fmt(slice s, char const *fmt, ...)
{
  va_list arg;
  va_start(arg, fmt);
  va_list arg_save;
  va_copy(arg_save, arg);
  size_t size = vsnprintf(NULL, 0, fmt, arg);
  if (s == NULL)
  {
    s = slice_alloc_mem(size);
  }
  else
  {
    s = slice_make_room_for(s, size);
  }
  vsnprintf(s, size, fmt, arg_save);
  va_end(arg_save);
  va_end(arg);
  return s;
}
slice slice_trim(slice s, const char *cset)
{
  char *start, *end, *sp, *ep;
  size_t len;

  sp = start = s;
  ep = end = s + sds_len(s) - 1;
  while (sp <= end && strchr(cset, *sp))
  {
    sp++;
  }
  while (ep > sp && strchr(cset, *ep))
  {
    ep--;
  }
  len = (sp > ep) ? 0 : ((ep - sp) + 1);
  if (s != sp)
  {
    memmove(s, sp, len);
  }
  s[len] = '\0';
  sds_set_len(s, len);
  return s;
}
void slice_toupper(slice s)
{
  size_t len = slice_len(s), j;

  for (j = 0; j < len; j++)
  {
    s[j] = toupper(s[j]);
  }
}
void slice_tolower(slice s)
{
  size_t len = slice_len(s), j;

  for (j = 0; j < len; j++)
  {
    s[j] = tolower(s[j]);
  }
}
void slice_reset(slice s)
{
  if (s != NULL)
  {
    slice_set_len(s, 0);
    s[0] = '\0';
  }
}

int slice_cmp(const slice s1, const slice s2)
{
  size_t l1, l2, minlen;
  int cmp;
  l1 = slcie_len(s1);
  l2 = slice_len(s2);
  minlen = (l1 < l2) ? l1 : l2;
  cmp = memcmp(s1, s2, minlen);
  if (cmp == 0)
  {
    return l1 > l2 ? 1 : (l1 < l2 ? -1 : 0);
  }
  return cmp;
}