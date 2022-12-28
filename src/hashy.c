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
  map->config.remember_keys = false;
  hashy_bucket_buffer_init(&map->buckets, map->capacity);
}

void hashy_map_init_v2(HashyMap* map, HashyMapConfig cfg) {
  int64_t capacity = OR(cfg.capacity, HASHY_DEFAULT_CAPACITY);
  cfg.capacity = capacity;

  hashy_map_init(map, capacity);

  if (cfg.remember_keys) {
    hashy_key_list_init(&map->keys, capacity);
  }

  map->config = cfg;
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


  HashyBucket* existing = hashy_map_get_bucket(map , key);

  if (existing)  {
    if (existing->key == 0) HASHY_WARNING_RETURN(0, stderr, "Corrupt hashmap, bucket is missing key.\n");
    existing->value = value;
    return existing->value;
  }

  if (map->config.remember_keys && map->keys.initialized) {
    if (existing == 0) {
      hashy_key_list_push(&map->keys, key);
    }
  }



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


  if (map->keys.initialized) {
    hashy_key_list_clear(&map->keys);
  }

  return 1;
}

void* hashy_map_unset(HashyMap* map, const char* key) {
  if (!map) HASHY_WARNING_RETURN(0, stderr, "Map is null.\n");
  if (!key) HASHY_WARNING_RETURN(0, stderr, "key is null.\n");

  HashyBucket* bucket = hashy_map_get_bucket(map, key);
  if (!bucket) return 0;


  if (map->config.remember_keys && map->keys.initialized) {
    hashy_key_list_remove(&map->keys, key);
  }

  if (bucket->key != 0) {
    free(bucket->key);
  }

  void* value = bucket->value;

  bucket->key = 0;
  bucket->value = 0;
  map->used = MAX(0, map->used - 1);
  bucket->initialized = false;

  return value;
}

int hashy_map_get_keys(HashyMap* map, HashyKeyList* out) {
  if (!map || !out) return 0;
  if (!out->initialized) {
    hashy_key_list_init(out, map->capacity / 2);
  }
  if (map->buckets.length <= 0) return 0;

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

int hashy_map_iterate(HashyMap* map, HashyIterator* it) {
  if (!map || !it) return 0;



  if (it->keys.length <= 0) {
    if (!hashy_map_get_keys(map, &it->keys)) {
      hashy_key_list_clear(&it->keys);
      return 0;
    }
  }


  if (it->i >= it->keys.length) {
    hashy_key_list_clear(&it->keys);
    return 0;
  }

  const char* key = it->keys.items[it->i];

  if (!key) {
    hashy_key_list_clear(&it->keys);
    return 0;
  }

  it->bucket = hashy_map_get_bucket(map, key);

  it->i++;

  if (it->bucket == 0) {
    hashy_key_list_clear(&it->keys);
  }

  return it->bucket != 0;
}
