#ifndef HASHY_H
#define HASHY_H
#include <hashy/bucket.h>
#include <stdbool.h>

typedef struct HASHY_MAP_STRUCT {
  HashyBucketBuffer buckets;
  bool initialized;
  int64_t capacity;
} HashyMap;

void hashy_map_init(HashyMap* map, int64_t capacity);

void* hashy_map_set(HashyMap* map, const char* key, void* value);

void* hashy_map_get(HashyMap* map, const char* key);
#endif
