/*************************************************************************
  > File Name: hashfn.h
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: 2020年04月22日 星期三 22时49分09秒
 ************************************************************************/

#ifndef _HASHFN_H
#define _HASHFN_H
#include <stdint.h>
#include <stdio.h>
uint32_t cstl_hash_crc16(const char* key, size_t key_length);
uint32_t cstl_hash_crc32(const char *key, size_t key_length);
uint32_t cstl_hash_crc32a(const char *key, size_t key_length);
uint32_t cstl_hash_fnv1_64(const char *key, size_t key_length);
uint32_t cstl_hash_hsieh(const char *key, size_t key_length);
uint32_t cstl_hash_jenkins(const char *key, size_t length);
uint32_t cstl_hash_fnv1a_64(const char *key, size_t key_length);
uint32_t cstl_hash_murmur(const char *key, size_t length);
uint32_t cstl_hash_fnv1_32(const char *key, size_t key_length);
uint32_t cstl_hash_fnv1a_32(const char *key, size_t key_length);
uint32_t cstl_hash_one_at_a_time(const char *key, size_t key_length);
uint32_t cstl_hash_jump_consistent(uint64_t key, int32_t num_buckets);
uint64_t cstl_hash_gfs(const char *msg, int len);
uint32_t cstl_hash_jump_consistent(uint64_t key, int32_t num_buckets);
#endif
