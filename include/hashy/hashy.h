#ifndef HASHY_H
#define HASHY_H
#include <hashy/bucket.h>
#include <stdbool.h>
#include <hashy/keylist.h>
#include <hashy/config.h>

struct HASHY_MAP_STRUCT;


typedef struct HASHY_MAP_STRUCT {
  struct {
    HashyBucket* items;
    uint64_t length;
  } buckets;
  

  int64_t num_inserts;
  int64_t num_unsets;
  int64_t num_collisions;
  
  bool initialized;
  HashyConfig config;
} HashyMap;

int hashy_map_init(HashyMap* map, HashyConfig cfg);

int hashy_map_clear(HashyMap* map);

int hashy_map_destroy(HashyMap* map);

int hashy_map_set(HashyMap* map, const char* key, void* value);
int hashy_map_unset(HashyMap* map, const char* key);

void* hashy_map_get(HashyMap* map, const char* key);

HashyBucket* hashy_map_get_bucket(HashyMap* map, const char* key);

int hashy_map_get_keys(HashyMap* map, HashyKeyList* out);

typedef struct {
  HashyBucket* bucket;
  HashyKeyList keys;
  int64_t i;
} HashyIterator;

int hashy_map_iterate(HashyMap* map, HashyIterator* it);

#endif
