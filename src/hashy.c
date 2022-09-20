#include <hashy/hashy.h>
#include <hashy/macros.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>





void hashy_map_init(HashyMap* map, int64_t capacity) {
  if (!map) return;
  if (map->initialized) HASHY_WARNING_RETURN(, stderr, "map already initialized.\n");
  map->initialized = true;
  map->capacity = OR(capacity, HASHY_DEFAULT_CAPACITY);

  map->buckets.initialized = false;
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


HashyMap* hashy_map_get_root(HashyMap* map) {
  if (!map) return 0;
  if (!map->root) return map;
  return hashy_map_get_root(map->root);
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

  if (!bucket->key) {
    if (!bucket->value && !bucket->map && !bucket->initialized) {
      hashy_bucket_init(bucket);
      map->used += 1;
    }
    bucket->key = strdup(key);
  }

  if (collision) {
    if (!bucket->map) {
      bucket->map = NEW(HashyMap);
      bucket->map->root = map;
      hashy_map_init(bucket->map, (map->capacity + (map->capacity / 2)));
    }
    return hashy_map_set(bucket->map, key, value);
  }



  bucket->value = value;

  return value;
}


HashyBucket* hashy_map_get_bucket(HashyMap* map, const char* key) {
  if (!map) HASHY_WARNING_RETURN(0, stderr, "map is null.\n");
  if (!map->initialized) HASHY_WARNING_RETURN(0, stderr, "map is not initialized.\n");
  if (!key) HASHY_WARNING_RETURN(0, stderr, "Key is null.\n");
  if (map->buckets.items == 0) return 0;

  uint64_t hash = hashy_hash(key) % map->capacity;

  HashyBucket* bucket = hashy_bucket_buffer_get(&map->buckets, hash);
  if (!bucket) HASHY_WARNING_RETURN(0, stderr, "Expected a bucket to exist.\n");

  if (bucket->key != 0 && strcmp(bucket->key, key) == 0) return bucket;

  return bucket->map ? hashy_map_get_bucket(bucket->map, key) : 0;
}

void* hashy_map_get(HashyMap* map, const char* key) {
  if (!map) HASHY_WARNING_RETURN(0, stderr, "map is null.\n");
  if (!map->initialized) HASHY_WARNING_RETURN(0, stderr, "map is not initialized.\n");
  if (!key) HASHY_WARNING_RETURN(0, stderr, "Key is null.\n");
  if (map->buckets.items == 0) return 0;

  HashyBucket* bucket = hashy_map_get_bucket(map, key);
  if (!bucket) return 0;
  return bucket->value;
}

int hashy_map_clear(HashyMap* map, bool free_values) {
  if (!map) return 0;

  hashy_bucket_buffer_clear(&map->buckets, free_values);
  map->used = 0;

  return 1;
}

void* hashy_map_unset(HashyMap* map, const char* key) {
  if (!map) HASHY_WARNING_RETURN(0, stderr, "Map is null.\n");
  if (!key) HASHY_WARNING_RETURN(0, stderr, "key is null.\n");

  HashyBucket* bucket = hashy_map_get_bucket(map, key);
  if (!bucket) return 0;

  if (bucket->key == 0 && bucket->map != 0) return hashy_map_unset(bucket->map, key);
  if (bucket->key == 0) HASHY_WARNING_RETURN(0, stderr, "Bucket has no key.\n");

  if (strcmp(bucket->key, key) != 0) {
    if (bucket->map) return hashy_map_unset(bucket->map, key);

    HASHY_WARNING_RETURN(0, stderr, "bucket key does not match.\n");
  }

  void* value = bucket->value;

  free(bucket->key);
  bucket->key = 0;
  bucket->value = 0;
  map->used = MAX(0, map->used - 1);

  return value;
}

int hashy_map_get_keys(HashyMap* map, HashyKeyList* out) {
  if (!map || !out) return 0;
  if (!out->initialized) {
    hashy_key_list_init(out, map->capacity / 2);
  }


  for (int64_t i = 0; i < map->buckets.length; i++) {
    HashyBucket bucket = map->buckets.items[i];

    if (bucket.key != 0) {
      hashy_key_list_push(out, bucket.key);
    }

    if (bucket.map != 0) {
      hashy_map_get_keys(bucket.map, out);
    }
  }

  return out->length > 0;
}
