/*************************************************************************
  > File Name: md5.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 2020年04月22日 星期三 22时48分36秒
 ************************************************************************/

#ifndef _MD5_H
#define _MD5_H
int md5_string(unsigned char *dest_str, unsigned int dest_len, char *md5_str);
int md5_file(const char *file_path, char *md5_str);
#endif
