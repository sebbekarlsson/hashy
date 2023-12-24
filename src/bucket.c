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
  //bucket->map = 0;
  bucket->pair = (HashyI642){0};
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
  bucket->pair = (HashyI642){0};
  bucket->hash = 0;
  bucket->is_set = false;

  /*
  if (bucket->map != 0) {
    if (bucket->config.destroy_recursion_on_clear) {
      hashy_map_destroy(bucket->map);
      free(bucket->map);
      bucket->map = 0;
    } else {
      hashy_map_clear(bucket->map);
    }
    }*/

  return 1;
}

int hashy_bucket_destroy(HashyBucket* bucket) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);

  if (bucket->value != 0 && bucket->config.free_values_on_destroy) {
    free(bucket->value);
    bucket->value = 0;
  }
  
  bucket->value = 0;
  bucket->pair = (HashyI642){0};
  bucket->index = 0;
  bucket->hash = 0;
  bucket->is_set = false;
  bucket->initialized = false;

  /*
  if (bucket->map != 0) {
    hashy_map_destroy(bucket->map);
    free(bucket->map);
    }

  bucket->map = 0;
  */
  return 1;
}

bool hashy_bucket_matches(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash) {
  return bucket->key.value[0] == key[0] && strcmp(bucket->key.value, key) == 0;
}

bool hashy_bucket_matchesi(HashyBucket* bucket, HashyI642 key, uint64_t index, uint64_t hash) {
  return bucket->pair.a == key.a && bucket->pair.b == key.b;
}

int hashy_bucket_set(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash, void* value, int64_t* num_collisions) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);
  HASHY_ASSERT_RETURN(num_collisions != 0, 0);

  if (bucket->is_set == false || hashy_bucket_matches(bucket, key, index, hash)) {
    if (bucket->value != 0 && bucket->config.free_values_on_overwrite) {
      free(bucket->value);
      bucket->value = 0;
    }
    bucket->value = value;
    if (bucket->is_set == false) {
      snprintf(&bucket->key.value[0], HASHY_MAX_KEY_LENGTH, "%s", key);
      //bucket->key = hashy_string_make(key);
    }
    bucket->index = index;
    bucket->hash = hash;
    bucket->is_set = true;
    return 1;
  }

  HASHY_ASSERT_RETURN(bucket->is_set == true, 0);

  return 0;
}

int hashy_bucket_unset(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);

  if (!hashy_bucket_matches(bucket, key, index, hash)) return 0;
  
  if (bucket->is_set && bucket->value != 0 && bucket->config.free_values_on_unset) {
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

void* hashy_bucket_get(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);

  if (bucket->is_set && hashy_bucket_matches(bucket, key, index, hash)) {
    return bucket->value;
  }

  return 0;
}

HashyBucket* hashy_bucket_get_bucket(HashyBucket* bucket, const char* key, uint64_t index, uint64_t hash) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);

  if (!(bucket->is_set && hashy_bucket_matches(bucket, key, index, hash))) return 0; 

  return bucket;
}


int hashy_bucket_seti(HashyBucket* bucket, HashyI642 key, uint64_t index, uint64_t hash, void* value, int64_t* num_collisions) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);
  HASHY_ASSERT_RETURN(num_collisions != 0, 0);

  if (bucket->is_set == false || hashy_bucket_matchesi(bucket, key, index, hash)) {
    if (bucket->value != 0 && bucket->config.free_values_on_overwrite) {
      free(bucket->value);
      bucket->value = 0;
    }
    bucket->value = value;
    if (bucket->is_set == false) {
      bucket->pair = key;
      //bucket->key = hashy_string_make(key);
    }
    bucket->index = index;
    bucket->hash = hash;
    bucket->is_set = true;
    return 1;
  }

  HASHY_ASSERT_RETURN(bucket->is_set == true, 0);

  return 0;
}

int hashy_bucket_unseti(HashyBucket* bucket,  HashyI642 key, uint64_t index, uint64_t hash) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);

  if (!hashy_bucket_matchesi(bucket, key, index, hash)) return 0;
  
  if (bucket->is_set && bucket->value != 0 && bucket->config.free_values_on_unset) {
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

void* hashy_bucket_geti(HashyBucket* bucket,  HashyI642 key, uint64_t index, uint64_t hash) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);

  if (bucket->is_set && hashy_bucket_matchesi(bucket, key, index, hash)) {
    return bucket->value;
  }

  return 0;
}

HashyBucket* hashy_bucket_geti_bucket(HashyBucket* bucket,  HashyI642 key, uint64_t index, uint64_t hash) {
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  HASHY_ASSERT_RETURN(bucket->initialized == true, 0);
  
  if (!(bucket->is_set && hashy_bucket_matchesi(bucket, key, index, hash))) return 0;

  return bucket;
}
