#include <hashy/bucket.h>
#include <hashy/hashy.h>
#include <hashy/macros.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int hashy_bucket_init(HashyBucket* bucket, HashyConfig cfg) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(cfg.capacity > 0, 0);
  if (bucket->initialized) return 1;
  
  hashy_string_clear(&bucket->key);
  bucket->map = 0;
  bucket->value = 0;
  bucket->hash = 0;
  bucket->index = 0;
  bucket->is_set = false;
  bucket->config = cfg;
  bucket->initialized = true;
  return 1;
}

int hashy_bucket_clear(HashyBucket* bucket) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  if (bucket->value != 0 && bucket->config.free_values_on_clear) {
    free(bucket->value);
    bucket->value = 0;
  }
  bucket->value = 0;
  hashy_string_clear(&bucket->key);
  bucket->index = 0;
  bucket->hash = 0;
  bucket->is_set = false;

  if (bucket->map != 0) {
    hashy_map_clear(bucket->map);
  }

  return 1;
}

int hashy_bucket_destroy(HashyBucket* bucket) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);

  if (bucket->value != 0 && bucket->config.free_values_on_destroy) {
    free(bucket->value);
    bucket->value = 0;
  }
  
  bucket->value = 0;
  hashy_string_clear(&bucket->key);
  bucket->index = 0;
  bucket->hash = 0;
  bucket->is_set = false;

  if (bucket->map != 0) {
    hashy_map_destroy(bucket->map);
    free(bucket->map);
  }

  bucket->map = 0;

  return 1;
}

static inline bool bucket_matches(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash) {
  if (!bucket->is_set) return true;
  return (bucket->hash == hash && bucket->index == index) && strcmp(bucket->key.value, key) == 0;
}

int hashy_bucket_set(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash, void* value, int64_t* num_collisions) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);
  HASHY_ASSERT_RETURN(num_collisions != 0, 0);

  if (bucket_matches(bucket, key, index, hash)) {
    if (bucket->value != 0 && bucket->config.free_values_on_overwrite) {
      free(bucket->value);
      bucket->value = 0;
    }
    bucket->value = value;
    bucket->key = bucket->is_set ? bucket->key : hashy_string_make(key);
    bucket->index = index;
    bucket->hash = hash;
    bucket->is_set = true;
    return 1;
  }

  if (bucket->map == 0) {
    bucket->map = (HashyMap*)calloc(1, sizeof(HashyMap));
  }

  HASHY_ASSERT_RETURN(bucket->map != 0, 0);

  if (!bucket->map->initialized) {
    HASHY_ASSERT_RETURN(hashy_map_init(bucket->map, bucket->config) == 1, 0);
  }

  int64_t n_collisions = *num_collisions;
  n_collisions += 1;
  *num_collisions = n_collisions;

  return hashy_map_set(bucket->map, key, value);
}

int hashy_bucket_unset(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);

  if (bucket->is_set && bucket_matches(bucket, key, index, hash)) {
    if (bucket->value != 0 && bucket->config.free_values_on_unset) {
      free(bucket->value);
      bucket->value = 0;
    }
    bucket->value = 0;
    bucket->index = 0;
    bucket->hash = 0;
    bucket->is_set = false;
    hashy_string_clear(&bucket->key);
    return 1;
  }

  if (bucket->map != 0) {
    return hashy_map_unset(bucket->map, key);
  }

  return 0;
}

void* hashy_bucket_get(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);

  if (bucket_matches(bucket, key, index, hash)) {
    return bucket->value;
  }

  if (bucket->map != 0) {
    return hashy_map_get(bucket->map, key);
  }

  return 0;
}

HashyBucket* hashy_bucket_get_bucket(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);

  if (bucket->is_set && bucket_matches(bucket, key, index, hash)) return bucket;

  if (bucket->map != 0) {
    return hashy_map_get_bucket(bucket->map, key);
  }

  return 0;
}
