#include <hashy/hashy.h>
#include <hashy/macros.h>
#include <hashy/constants.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t hashy_hash_func(const char* value) {
  uint64_t hash = 0;
  unsigned char* str = (unsigned char*)value;
  int c = 0;

  while ((c = *str++)) {
    hash = (c + (hash << 6) + (hash << 16) - hash);
  }

  return hash;
}

int hashy_map_init(HashyMap* map, HashyConfig cfg) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  if (map->initialized) return 1;
  cfg.capacity = cfg.capacity ? cfg.capacity : HASHY_DEFAULT_CAPACITY;

  map->buckets.items = (HashyBucket*)calloc(cfg.capacity, sizeof(HashyBucket));
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  map->buckets.length = cfg.capacity;

  map->num_inserts = 0;
  map->num_unsets = 0;
  map->num_collisions = 0;
  map->config = cfg;
  map->initialized = true;
  return 1;
}

int hashy_map_clear(HashyMap* map) {
  HASHY_ASSERT_RETURN(map != 0, 0);

  map->num_inserts = 0;
  map->num_collisions = 0;
  map->num_unsets = 0;

  if (map->buckets.items != 0) {
    HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

    for (uint64_t i = 0; i < map->buckets.length; i++) {
      hashy_bucket_clear(&map->buckets.items[i]);
    }
  }

  return 1;
}

int hashy_map_destroy(HashyMap* map) {
  HASHY_ASSERT_RETURN(map != 0, 0);

  map->num_inserts = 0;
  map->num_unsets = 0;
  map->num_collisions = 0;

  if (map->buckets.items != 0) {
    HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

    for (uint64_t i = 0; i < map->buckets.length; i++) {
      hashy_bucket_destroy(&map->buckets.items[i]);
    }

    free(map->buckets.items);
  }

  map->buckets.items = 0;
  map->buckets.length = 0;

  map->initialized = false;
  
  return 1;
}

int hashy_map_set(HashyMap* map, const char* key, void* value) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  uint64_t hash = hashy_hash_func(key);
  uint64_t index = hash % map->buckets.length;
  HashyBucket* bucket = &map->buckets.items[index];
  if (!bucket->initialized) {
    HASHY_ASSERT_RETURN(hashy_bucket_init(bucket, map->config) == 1, 0);
  }

  bool was_set = bucket->is_set;

  HASHY_ASSERT_RETURN(hashy_bucket_set(bucket, key, index, hash, value, &map->num_collisions) == 1, 0);

  if (!was_set) {
    map->num_inserts += 1;
  }

  return 1;
}

int hashy_map_unset(HashyMap* map, const char* key) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  uint64_t hash = hashy_hash_func(key);
  uint64_t index = hash % map->buckets.length;
  HashyBucket* bucket = &map->buckets.items[index];

  if (!bucket->initialized) {
    HASHY_ASSERT_RETURN(hashy_bucket_init(bucket, map->config) == 1, 0);
  }

  bool was_set = bucket->is_set;

  if (!hashy_bucket_unset(bucket, key, index, hash)) return 0;

  if (was_set) {
    map->num_unsets += 1;
  }

  return 1;
}

void* hashy_map_get(HashyMap* map, const char* key) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  uint64_t hash = hashy_hash_func(key);
  uint64_t index = hash % map->buckets.length;
  HashyBucket* bucket = &map->buckets.items[index];
  if (!bucket->initialized) {
    HASHY_ASSERT_RETURN(hashy_bucket_init(bucket, map->config) == 1, 0);
  }

  return hashy_bucket_get(bucket, key, index, hash);
}

HashyBucket* hashy_map_get_bucket(HashyMap* map, const char* key) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  uint64_t hash = hashy_hash_func(key);
  uint64_t index = hash % map->buckets.length;
  HashyBucket* bucket = &map->buckets.items[index];
  if (!bucket->initialized) {
    HASHY_ASSERT_RETURN(hashy_bucket_init(bucket, map->config) == 1, 0);
  }

  return hashy_bucket_get_bucket(bucket, key, index, hash);
}
