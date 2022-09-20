#ifndef HASHY_H
#define HASHY_H
#include <hashy/bucket.h>
#include <stdbool.h>
#include <hashy/keylist.h>

struct HASHY_MAP_STRUCT;

typedef struct HASHY_MAP_STRUCT {
  HashyBucketBuffer buckets;
  bool initialized;
  int64_t capacity;
  int64_t used;

  struct HASHY_MAP_STRUCT* root;
} HashyMap;


void hashy_map_init(HashyMap* map, int64_t capacity);

void* hashy_map_set(HashyMap* map, const char* key, void* value);

void* hashy_map_get(HashyMap* map, const char* key);

HashyBucket* hashy_map_get_bucket(HashyMap* map, const char* key);


int hashy_map_clear(HashyMap* map, bool free_values);

void* hashy_map_unset(HashyMap* map, const char* key);


int hashy_map_get_keys(HashyMap* map, HashyKeyList* out);
#endif
