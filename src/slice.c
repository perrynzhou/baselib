/*************************************************************************
    > File Name: slice.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Monday, September 07, 2020 AM10:26:15
 ************************************************************************/
#include "../inc/slice.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
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
#define SLICE_MAX_INT_T_VALUE 9223372036854775807

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
static const uint32_t crc32tab[256] = {
    0x00000000,
    0x77073096,
    0xee0e612c,
    0x990951ba,
    0x076dc419,
    0x706af48f,
    0xe963a535,
    0x9e6495a3,
    0x0edb8832,
    0x79dcb8a4,
    0xe0d5e91e,
    0x97d2d988,
    0x09b64c2b,
    0x7eb17cbd,
    0xe7b82d07,
    0x90bf1d91,
    0x1db71064,
    0x6ab020f2,
    0xf3b97148,
    0x84be41de,
    0x1adad47d,
    0x6ddde4eb,
    0xf4d4b551,
    0x83d385c7,
    0x136c9856,
    0x646ba8c0,
    0xfd62f97a,
    0x8a65c9ec,
    0x14015c4f,
    0x63066cd9,
    0xfa0f3d63,
    0x8d080df5,
    0x3b6e20c8,
    0x4c69105e,
    0xd56041e4,
    0xa2677172,
    0x3c03e4d1,
    0x4b04d447,
    0xd20d85fd,
    0xa50ab56b,
    0x35b5a8fa,
    0x42b2986c,
    0xdbbbc9d6,
    0xacbcf940,
    0x32d86ce3,
    0x45df5c75,
    0xdcd60dcf,
    0xabd13d59,
    0x26d930ac,
    0x51de003a,
    0xc8d75180,
    0xbfd06116,
    0x21b4f4b5,
    0x56b3c423,
    0xcfba9599,
    0xb8bda50f,
    0x2802b89e,
    0x5f058808,
    0xc60cd9b2,
    0xb10be924,
    0x2f6f7c87,
    0x58684c11,
    0xc1611dab,
    0xb6662d3d,
    0x76dc4190,
    0x01db7106,
    0x98d220bc,
    0xefd5102a,
    0x71b18589,
    0x06b6b51f,
    0x9fbfe4a5,
    0xe8b8d433,
    0x7807c9a2,
    0x0f00f934,
    0x9609a88e,
    0xe10e9818,
    0x7f6a0dbb,
    0x086d3d2d,
    0x91646c97,
    0xe6635c01,
    0x6b6b51f4,
    0x1c6c6162,
    0x856530d8,
    0xf262004e,
    0x6c0695ed,
    0x1b01a57b,
    0x8208f4c1,
    0xf50fc457,
    0x65b0d9c6,
    0x12b7e950,
    0x8bbeb8ea,
    0xfcb9887c,
    0x62dd1ddf,
    0x15da2d49,
    0x8cd37cf3,
    0xfbd44c65,
    0x4db26158,
    0x3ab551ce,
    0xa3bc0074,
    0xd4bb30e2,
    0x4adfa541,
    0x3dd895d7,
    0xa4d1c46d,
    0xd3d6f4fb,
    0x4369e96a,
    0x346ed9fc,
    0xad678846,
    0xda60b8d0,
    0x44042d73,
    0x33031de5,
    0xaa0a4c5f,
    0xdd0d7cc9,
    0x5005713c,
    0x270241aa,
    0xbe0b1010,
    0xc90c2086,
    0x5768b525,
    0x206f85b3,
    0xb966d409,
    0xce61e49f,
    0x5edef90e,
    0x29d9c998,
    0xb0d09822,
    0xc7d7a8b4,
    0x59b33d17,
    0x2eb40d81,
    0xb7bd5c3b,
    0xc0ba6cad,
    0xedb88320,
    0x9abfb3b6,
    0x03b6e20c,
    0x74b1d29a,
    0xead54739,
    0x9dd277af,
    0x04db2615,
    0x73dc1683,
    0xe3630b12,
    0x94643b84,
    0x0d6d6a3e,
    0x7a6a5aa8,
    0xe40ecf0b,
    0x9309ff9d,
    0x0a00ae27,
    0x7d079eb1,
    0xf00f9344,
    0x8708a3d2,
    0x1e01f268,
    0x6906c2fe,
    0xf762575d,
    0x806567cb,
    0x196c3671,
    0x6e6b06e7,
    0xfed41b76,
    0x89d32be0,
    0x10da7a5a,
    0x67dd4acc,
    0xf9b9df6f,
    0x8ebeeff9,
    0x17b7be43,
    0x60b08ed5,
    0xd6d6a3e8,
    0xa1d1937e,
    0x38d8c2c4,
    0x4fdff252,
    0xd1bb67f1,
    0xa6bc5767,
    0x3fb506dd,
    0x48b2364b,
    0xd80d2bda,
    0xaf0a1b4c,
    0x36034af6,
    0x41047a60,
    0xdf60efc3,
    0xa867df55,
    0x316e8eef,
    0x4669be79,
    0xcb61b38c,
    0xbc66831a,
    0x256fd2a0,
    0x5268e236,
    0xcc0c7795,
    0xbb0b4703,
    0x220216b9,
    0x5505262f,
    0xc5ba3bbe,
    0xb2bd0b28,
    0x2bb45a92,
    0x5cb36a04,
    0xc2d7ffa7,
    0xb5d0cf31,
    0x2cd99e8b,
    0x5bdeae1d,
    0x9b64c2b0,
    0xec63f226,
    0x756aa39c,
    0x026d930a,
    0x9c0906a9,
    0xeb0e363f,
    0x72076785,
    0x05005713,
    0x95bf4a82,
    0xe2b87a14,
    0x7bb12bae,
    0x0cb61b38,
    0x92d28e9b,
    0xe5d5be0d,
    0x7cdcefb7,
    0x0bdbdf21,
    0x86d3d2d4,
    0xf1d4e242,
    0x68ddb3f8,
    0x1fda836e,
    0x81be16cd,
    0xf6b9265b,
    0x6fb077e1,
    0x18b74777,
    0x88085ae6,
    0xff0f6a70,
    0x66063bca,
    0x11010b5c,
    0x8f659eff,
    0xf862ae69,
    0x616bffd3,
    0x166ccf45,
    0xa00ae278,
    0xd70dd2ee,
    0x4e048354,
    0x3903b3c2,
    0xa7672661,
    0xd06016f7,
    0x4969474d,
    0x3e6e77db,
    0xaed16a4a,
    0xd9d65adc,
    0x40df0b66,
    0x37d83bf0,
    0xa9bcae53,
    0xdebb9ec5,
    0x47b2cf7f,
    0x30b5ffe9,
    0xbdbdf21c,
    0xcabac28a,
    0x53b39330,
    0x24b4a3a6,
    0xbad03605,
    0xcdd70693,
    0x54de5729,
    0x23d967bf,
    0xb3667a2e,
    0xc4614ab8,
    0x5d681b02,
    0x2a6f2b94,
    0xb40bbe37,
    0xc30c8ea1,
    0x5a05df1b,
    0x2d02ef8d,
};
static uint32_t hash_crc32(const char *key, size_t key_length)
{
  uint64_t x;
  uint32_t crc = UINT32_MAX;

  for (x = 0; x < key_length; x++)
  {
    crc = (crc >> 8) ^ crc32tab[(crc ^ (uint64_t)key[x]) & 0xff];
  }

  return ((~crc) >> 16) & 0x7fff;
}
uint32_t slice_hash(slice s)
{
  const char *data = (const char *)(s);
  return hash_crc32(data, slice_len(s));
}
int slice_atoi(slice s)
{
  const char *data = (const char *)s;
  int8_t sign = 1;
  int i = 0;
  if (data[i] == '-' || data[i] == '+')
  {
    sign = (data[i] == '-') ? -1 : 1;
    i++;
  }
  int tmp = 0;
  size_t len = slice_len(s);
  if (i > len || data[i] < '0' || data[i] > '9')
  {
    return -1;
  }
  for (; i < len; i++)
  {
    if (data[i] < '0' || data[i] > '9')
    {
      break;
    }
    tmp = tmp * 10 + (s[i] - '0');
    if ((sign * tmp) > INT64_MAX)
    {
      tmp = INT32_MAX;
      break;
    }
    if ((sign * tmp) < INT64_MIN)
    {
      tmp = INT32_MIN;
      break;
    }
  }
  if (tmp > INT32_MIN && tmp < INT32_MAX)
  {
    return tmp;
  }
  return sign * tmp;
}
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
static void slice_destroy_internal(slice s)
{
  void *data = NULL;
  unsigned char flags = s[-1];
  unsigned char ret = (flags & SLICE_TYPE_MASK);
  if (ret == SLICE_TYPE_5)
  {
    SLICE_HDR_VAR(5, s);
    data = sh;
  }
  else if (ret == SLICE_TYPE_8)
  {
    SLICE_HDR_VAR(8, s);
    data = sh;
  }
  else if (ret == SLICE_TYPE_16)
  {
    SLICE_HDR_VAR(16, s);
    data = sh;
  }
  else if (ret == SLICE_TYPE_32)
  {
    SLICE_HDR_VAR(32, s);
    data = sh;
  }
  else if (ret == SLICE_TYPE_64)
  {
    SLICE_HDR_VAR(64, s);
    data = sh;
  }
  if (data != NULL)
  {
    free(data);
    s = NULL;
  }
}
void slice_destroy(slice s) {
  slice_destroy_internal(s);
}
void slice_deinit(slice *s) {
  slice tmp = *s;
  slice_destroy_internal(tmp);
  *s = NULL;
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
  {
    type = SLICE_TYPE_8;
  }
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
    s = slice_make_room_for(s, len);
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
int slice_init(slice *s,const char *str) {
  slice temp = slice_create(str);
  if (temp == NULL)
  {
    return -1;
  }
  *s = temp;
  return 0;
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
  ep = end = s + slice_len(s) - 1;
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
  slice_set_len(s, len);
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
  l1 = slice_len(s1);
  l2 = slice_len(s2);
  minlen = (l1 < l2) ? l1 : l2;
  cmp = memcmp(s1, s2, minlen);
  if (cmp == 0)
  {
    return l1 > l2 ? 1 : (l1 < l2 ? -1 : 0);
  }
  return cmp;
}
#ifdef TEST
int main(void)
{
  slice s = slice_create("-add");
  fprintf(stdout, "s=%s,ret=%d\n", (char *)s, slice_atoi(s));
  s = slice_create("+add");
  fprintf(stdout, "s=%s,ret=%d,value=%ld,addr=%p\n", (char *)s, slice_atoi(s));
  s = slice_create("-12ad");
  fprintf(stdout, "s=%s,ret=%d,value=%ld,addr=%p\n", (char *)s, slice_atoi(s));
  s = slice_create("+1024ad");
  fprintf(stdout, "s=%s,ret=%d,value=%ld,addr=%p\n", (char *)s, slice_atoi(s));
  return 0;
}
#endif