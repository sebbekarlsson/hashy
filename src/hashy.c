#include <hashy/hashy.h>
#include <hashy/macros.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


void hashy_map_init(HashyMap* map, int64_t capacity) {
  if (!map) return;
  if (map->initialized) return;
  map->initialized = true;
  map->capacity = OR(capacity, HASHY_DEFAULT_CAPACITY);
  hashy_bucket_buffer_init(&map->buckets, map->capacity);
}

static uint64_t hashy_hash(const char* value) {
  uint64_t hash = 0;
  unsigned char* str = (unsigned char*)value;
  int c = 0;

  while ((c = *str++)) {
    hash = (c + (hash << 6) + (hash << 16) - hash);
  }

  return hash;
}

void* hashy_map_set(HashyMap* map, const char* key, void* value) {
  if (!map) return 0;
  if (!map->initialized) HASHY_WARNING_RETURN(0, stderr, "Hashmap not initialized.\n");
  if (!key || !value) HASHY_WARNING_RETURN(0, stderr, "Key or value is null.\n");


  uint64_t hash = hashy_hash(key) % map->capacity;

  if (!map->buckets.items) {
    hashy_bucket_buffer_grow(&map->buckets, map->capacity);
  }

  HashyBucket* bucket = hashy_bucket_buffer_get(&map->buckets, hash);
  if (!bucket) HASHY_WARNING_RETURN(0, stderr, "Expected a bucket to exist.\n");

  bool collision = bucket->key != 0 && bucket->value != 0 && strcmp(bucket->key, key) != 0;

  if (collision) {
    if (!bucket->map) {
      bucket->map = NEW(HashyMap);
      hashy_map_init(bucket->map, map->capacity);
    }
    return hashy_map_set(bucket->map, key, value);
  }

  if (!bucket->key) {
    bucket->key = strdup(key);
  }

  bucket->value = value;

  return value;
}

void* hashy_map_get(HashyMap* map, const char* key) {
  if (!map) HASHY_WARNING_RETURN(0, stderr, "map is null.\n");
  if (!map->initialized) HASHY_WARNING_RETURN(0, stderr, "map is not initialized.\n");
  if (!key) HASHY_WARNING_RETURN(0, stderr, "Key is null.\n");

  uint64_t hash = hashy_hash(key) % map->capacity;

  HashyBucket* bucket = hashy_bucket_buffer_get(&map->buckets, hash);

  if (!bucket) HASHY_WARNING_RETURN(0, stderr, "Expected a bucket to exist.\n");

  if (bucket->key == 0 && bucket->map != 0) return hashy_map_get(bucket->map, key);
  if (bucket->key && strcmp(bucket->key, key) != 0 && bucket->map != 0) return hashy_map_get(bucket->map, key);

  return bucket->value;
}
