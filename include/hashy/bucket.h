#ifndef HASHY_BUCKET_H
#define HASHY_BUCKET_H
#include <stdint.h>
#include <stdbool.h>
#include <hashy/constants.h>
#include <hashy/string.h>
#include <hashy/config.h>
#include <hashy/i2.h>


struct HASHY_MAP_STRUCT;
struct HASHY_BUCKET_BUFFER;

typedef struct {
  HashyString key;
  HashyI642 pair;
  HashyConfig config;

  uint64_t index;
  uint64_t hash;
  
  //struct HASHY_MAP_STRUCT* map;
  void* value;
  bool is_set;
  bool initialized;
} HashyBucket;

int hashy_bucket_init(HashyBucket* bucket, HashyConfig cfg);

int hashy_bucket_clear(HashyBucket* bucket);

int hashy_bucket_destroy(HashyBucket* bucket);

///////////////////////////////////////////////////

int hashy_bucket_set(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash, void* value, int64_t* num_collisions);

int hashy_bucket_unset(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash);

void* hashy_bucket_get(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash);

HashyBucket* hashy_bucket_get_bucket(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash);

bool hashy_bucket_matches(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int hashy_bucket_seti(HashyBucket* bucket, HashyI642 key, uint64_t index, uint64_t hash, void* value, int64_t* num_collisions);

int hashy_bucket_unseti(HashyBucket* bucket, HashyI642 key, uint64_t index, uint64_t hash);

void* hashy_bucket_geti(HashyBucket* bucket, HashyI642 key, uint64_t index, uint64_t hash);

HashyBucket* hashy_bucket_geti_bucket(HashyBucket* bucket, HashyI642 key, uint64_t index, uint64_t hash);

bool hashy_bucket_matchesi(HashyBucket* bucket, HashyI642 key, uint64_t index, uint64_t hash);


///////////////////////////////////////////////////


#endif
