/*************************************************************************
    > File Name: util.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Thursday, January 28, 2021 AM08:16:39
 ************************************************************************/

#ifndef _UTIL_H
#define _UTIL_H
#define ch_tolower(c) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : (c))
#define ch_toupper(c)  ((c>='a'&&c<='z')?(c&~0x20:(c)))
inline void str_tolower(char *str)
{
  if (str != NULL)
  {
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
      ch_tolower(str[i]);
    }
  }
}
inline void str_toupper(char *str)
{
  if (str != NULL)
  {
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
      ch_toupper(str[i]);
    }
  }
}
#endif
