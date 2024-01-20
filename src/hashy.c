#include <hashy/hashy.h>
#include <hashy/macros.h>
#include <hashy/constants.h>
#include <hashy/cantor.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static inline void lock_mutex(HashyMap* map) {
  if (map == 0 || map->config.threadsafe == false) return;
  hashy_mutex_lock(&map->lock);
}

static inline void unlock_mutex(HashyMap* map) {
  if (map == 0 || map->config.threadsafe == false) return;
  hashy_mutex_unlock(&map->lock);
}

static inline void destroy_mutex(HashyMap* map) {
  if (map == 0 || map->config.threadsafe == false) return;
  hashy_mutex_destroy(&map->lock);
}

static inline HashyMap* get_root(HashyMap* map) {
  if (map->root != 0) return map->root;
  return map;
}

static inline HashyMap* get_last(HashyMap* map) {
  HashyMap* next = map->next;

  while (next != 0) {

    if (next->next == 0) return next;
    
    next = next->next;
  }

  return next;
}

static uint64_t hashy_hash_func(const char* value, uint64_t capacity) {
  uint64_t hash = 0;
  unsigned char* str = (unsigned char*)value;
  int c = 0;

  while ((c = *str++)) {
    hash = (c + (hash << 6) + (hash << 16) - hash);
  }

  return hash;
}

static uint64_t hashy_hashi_func(HashyI642 value, uint64_t capacity) {
  uint64_t a = (uint64_t)value.a;
  uint64_t b = (uint64_t)value.b;

  uint64_t hash = ((uint64_t)hashy_cantor(a, b));
  //hash = ((hash << 6) + (hash << 16) - hash);

  return hash;
}

int hashy_map_init(HashyMap* map, HashyConfig cfg) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  if (map->initialized) return 1;
  cfg.capacity = cfg.capacity ? cfg.capacity : HASHY_DEFAULT_CAPACITY;

  if (cfg.threadsafe) {
    if (!hashy_mutex_init(&map->lock)) {
      HASHY_WARNING_RETURN(0, stderr, "Failed to create mutex.\n");
    }
  }

  map->buckets.items = (HashyBucket*)calloc(cfg.capacity, sizeof(HashyBucket));
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  map->buckets.length = cfg.capacity;


  map->is_clear = true;
  map->num_inserts = 0;
  map->num_unsets = 0;
  map->num_collisions = 0;
  map->num_pages = 0;
  map->config = cfg;
  map->initialized = true;
  return 1;
}

#define HASHY_USE_OLD_CLEAR 0

#if HASHY_USE_OLD_CLEAR
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

  if (map->next != 0) {
    if (map->config.free_linked_on_clear) {
      hashy_map_destroy(map->next);
      free(map->next);
      map->next = 0;
      map->num_pages = MAX(0, map->num_pages-1);
    } else {
      hashy_map_clear(map->next);
    }
  }

  return 1;
}
#else

static inline int hashy_map_clear_(HashyMap* map) {
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

  if (map->next != 0) {
    HashyMap* last = get_last(map);

    HASHY_ASSERT_RETURN(last != 0, 0);
    HASHY_ASSERT_RETURN(last != map, 0);
    HashyMap* next = last;

    while (next != 0 && next != map) {
      HashyMap* tmp = next->prev;
      if (tmp == 0) break;
      HASHY_ASSERT_CONTINUE(tmp != next);
      HASHY_ASSERT_CONTINUE(next != map);
      HASHY_ASSERT_CONTINUE(next == tmp->next);

      if (map->config.free_linked_on_clear) {
        hashy_map_destroy(next);
        free(next); // next is (tmp->next)
        tmp->next = 0;
        map->num_pages -= 1;
        map->num_pages = MAX(map->num_pages, 0);
      } else if (next->is_clear == false) {
        hashy_map_clear(next);
        next->is_clear = true;
      }

      next = tmp;
    }
  }

  return 1;
}

int hashy_map_clear(HashyMap* map) {
  lock_mutex(map);
  int status = hashy_map_clear_(map);
  unlock_mutex(map);
  return status;
}

#endif

static inline int hashy_map_destroy_(HashyMap* map) {
  HASHY_ASSERT_RETURN(map != 0, 0);

  map->num_inserts = 0;
  map->num_unsets = 0;
  map->num_collisions = 0;
  map->num_pages = 0;

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

  if (map->next != 0) {
    hashy_map_destroy(map->next);
    free(map->next);
  }

  map->next = 0;
  map->root = 0;
  map->is_clear = true;
  
  return 1;
}

int hashy_map_destroy(HashyMap* map) {
  lock_mutex(map);
  int status = hashy_map_destroy_(map);
  unlock_mutex(map);
  return status;
}

int64_t hashy_map_count_pages(HashyMap* map) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  if (map->initialized == false) return 0;

  HashyMap* next = map->next;
  int64_t count = 0;

  while (next != 0) {
    count += 1;
    next = next->next;
  }

  return count;
}

typedef struct {
  uint64_t hash;
  uint64_t index;
} HashyHash;

static inline HashyBucket* find_bucket_for_key(HashyMap* map, const char* key, HashyHash* out, bool create) {
  uint64_t hash = hashy_hash_func(key, map->buckets.length);
  uint64_t index = hash % map->buckets.length;
  HASHY_ASSERT_RETURN(index >= 0, 0);
  HASHY_ASSERT_RETURN(index < map->buckets.length, 0);
  HashyBucket* bucket = &map->buckets.items[index];
  out->hash = hash;
  out->index = index;

  if (!create) {
    if (bucket->initialized == false || !hashy_bucket_matches(bucket, key, index, hash)) return 0;
    return bucket;
  }

  
  if (!bucket->initialized) {
    HASHY_ASSERT_RETURN(hashy_bucket_init(bucket, map->config) == 1, 0);
    return bucket;
  }

  if (bucket->is_set == false) return bucket;
  if (!hashy_bucket_matches(bucket, key, index, hash)) return 0;

  return bucket;
}

static inline HashyBucket* find_bucket_for_keyi(HashyMap* map, HashyI642 key, HashyHash* out, bool create) {
  uint64_t hash = hashy_hashi_func(key, map->buckets.length);
  uint64_t index = hash % map->buckets.length;
  HASHY_ASSERT_RETURN(index >= 0, 0);
  HASHY_ASSERT_RETURN(index < map->buckets.length, 0);
  HashyBucket* bucket = &map->buckets.items[index];
  out->hash = hash;
  out->index = index;

  if (!create) {
    if (!hashy_bucket_matchesi(bucket, key, index, hash)) return 0;

    if (!bucket->initialized) {
      HASHY_ASSERT_RETURN(hashy_bucket_init(bucket, map->config) == 1, 0);
    }
    
    return bucket;
  }

  
  if (!bucket->initialized) {
    HASHY_ASSERT_RETURN(hashy_bucket_init(bucket, map->config) == 1, 0);
    return bucket;
  }

  if (bucket->is_set == false) return bucket;
  if (!hashy_bucket_matchesi(bucket, key, index, hash)) return 0;

  return bucket;
}

static inline HashyBucket* find_bucket(HashyMap* map, const char* key, HashyHash* out, bool create) {
  HashyBucket* bucket = find_bucket_for_key(map, key, out, create);
  if (bucket != 0) return bucket;

  HashyMap* next = map->next;
  HashyMap* prev = map;

  while (next != 0) {
    bucket = find_bucket_for_key(next, key, out, create);
    if (bucket != 0) {
      return bucket;
    }

    if (next->next == 0) {
      prev = next;
    }
    next = next->next;
  }

  HASHY_ASSERT_RETURN(bucket == 0, 0);

  if (!create) return 0;

  HASHY_ASSERT_RETURN(prev != 0, 0);
  HASHY_ASSERT_RETURN(prev->next == 0, 0); 

  prev->next = (HashyMap*)calloc(1, sizeof(HashyMap));
  HASHY_ASSERT_RETURN(prev->next != 0, 0);
  HashyConfig next_config = map->config;
  next_config.threadsafe = false;
  next_config.capacity += map->config.capacity / 2;
  HASHY_ASSERT_RETURN(hashy_map_init(prev->next, next_config) == 1, 0);
  prev->next->root = map;
  prev->next->prev = prev;

  HashyMap* root = get_root(map);
  root->num_collisions += 1;
  root->num_pages += 1;

  bucket = find_bucket_for_key(prev->next, key, out, true);
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  

  return bucket;
}

static inline HashyBucket* find_bucketi(HashyMap* map, HashyI642 key, HashyHash* out, bool create, bool is_get) {
  HashyBucket* bucket = find_bucket_for_keyi(map, key, out, create);
  if (bucket != 0) return bucket;

  HashyMap* next = map->next;
  HashyMap* prev = map;

  while (next != 0) {
    bucket = find_bucket_for_keyi(next, key, out, create);
    if (bucket != 0) {
      if (!(is_get && bucket->is_set == false)) {
        return bucket;
      }
    }

    if (next->next == 0) {
      prev = next;
    }
    next = next->next;
  }

  HASHY_ASSERT_RETURN(bucket == 0, 0);

  if (!create) return 0;

  HASHY_ASSERT_RETURN(prev != 0, 0);
  HASHY_ASSERT_RETURN(prev->next == 0, 0); 

  prev->next = (HashyMap*)calloc(1, sizeof(HashyMap));
  HASHY_ASSERT_RETURN(prev->next != 0, 0);
  HashyConfig next_config = map->config;
  next_config.capacity += map->config.capacity / 2;
  HASHY_ASSERT_RETURN(hashy_map_init(prev->next, next_config) == 1, 0);
  prev->next->root = map;
  prev->next->prev = prev;

  HashyMap* root = get_root(map);
  root->num_collisions += 1;
  root->num_pages += 1;

  bucket = find_bucket_for_keyi(prev->next, key, out, true);
  HASHY_ASSERT_RETURN(bucket != 0, 0);
  

  return bucket;
}

static inline int hashy_map_set_(HashyMap* map, const char* key, void* value) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length == map->config.capacity, 0);

  HashyHash bhash = {0};
  HashyBucket* bucket = find_bucket(map, key, &bhash, true);
  HASHY_ASSERT_RETURN(bucket != 0, 0);

  bool was_set = bucket->is_set;

  HASHY_ASSERT_RETURN(hashy_bucket_set(bucket, key, bhash.index, bhash.hash, value, &map->num_collisions) == 1, 0);

  if (!was_set) {
    map->num_inserts += 1;
  }

  map->is_clear = false;

  return 1;
}

int hashy_map_set(HashyMap* map, const char* key, void* value) {
  lock_mutex(map);
  int status = hashy_map_set_(map, key, value);
  unlock_mutex(map);
  return status;
}

static inline int hashy_map_unset_(HashyMap* map, const char* key) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  
  HashyHash bhash = {0};
  HashyBucket* bucket = find_bucket(map, key, &bhash, false);
  if (bucket == 0) return 0;

  bool was_set = bucket->is_set;

  if (!hashy_bucket_unset(bucket, key, bhash.index, bhash.hash)) return 0;

  if (was_set) {
    map->num_unsets += 1;
  }

  return 1;
}

int hashy_map_unset(HashyMap* map, const char* key) {
  lock_mutex(map);
  int status = hashy_map_unset_(map, key);
  unlock_mutex(map);
  return status;
}

static inline void* hashy_map_get_(HashyMap* map, const char* key) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  
  HashyHash bhash = {0};
  HashyBucket* bucket = find_bucket(map, key, &bhash, false);
  if (bucket == 0) return 0;

  return hashy_bucket_get(bucket, key, bhash.index, bhash.hash);
}

void* hashy_map_get(HashyMap* map, const char* key) {
  lock_mutex(map);
  void* ptr = hashy_map_get_(map, key);
  unlock_mutex(map);
  return ptr;
}

HashyBucket* hashy_map_get_bucket(HashyMap* map, const char* key) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(key != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  HashyHash bhash = {0};
  return find_bucket(map, key, &bhash, false);
}


static inline int hashy_map_seti_(HashyMap* map, HashyI642 key, void* value) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length == map->config.capacity, 0);

  HashyHash bhash = {0};
  HashyBucket* bucket = find_bucketi(map, key, &bhash, true, false);
  HASHY_ASSERT_RETURN(bucket != 0, 0);

  bool was_set = bucket->is_set;

  HASHY_ASSERT_RETURN(hashy_bucket_seti(bucket, key, bhash.index, bhash.hash, value, &map->num_collisions) == 1, 0);

  if (!was_set) {
    map->num_inserts += 1;
  }

  return 1;
}

int hashy_map_seti(HashyMap* map, HashyI642 key, void* value) {
  lock_mutex(map);
  int status = hashy_map_seti_(map, key, value);
  unlock_mutex(map);
  return status;
}

static inline int hashy_map_unseti_(HashyMap* map, HashyI642 key) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  HashyHash bhash = {0};
  HashyBucket* bucket = find_bucketi(map, key, &bhash, false, false);
  if (bucket == 0) return 0;

  bool was_set = bucket->is_set;

  if (!hashy_bucket_unseti(bucket, key, bhash.index, bhash.hash)) return 0;

  if (was_set) {
    map->num_unsets += 1;
  }

  return 1;
}
int hashy_map_unseti(HashyMap* map, HashyI642 key) {
  lock_mutex(map);
  int status = hashy_map_unseti_(map, key);
  unlock_mutex(map);
  return status;
}
static inline void* hashy_map_geti_(HashyMap* map, HashyI642 key) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  
  HashyHash bhash = {0};
  HashyBucket* bucket = find_bucketi(map, key, &bhash, false, true);
  if (bucket == 0) return 0;

  return hashy_bucket_geti(bucket, key, bhash.index, bhash.hash);
}

void* hashy_map_geti(HashyMap* map, HashyI642 key) {
  lock_mutex(map);
  void* ptr = hashy_map_geti_(map, key);
  unlock_mutex(map);
  return ptr;
}

HashyBucket* hashy_map_get_bucketi(HashyMap* map, HashyI642 key) {
  HASHY_ASSERT_RETURN(map != 0, 0);
  HASHY_ASSERT_RETURN(map->initialized == true, 0);
  HASHY_ASSERT_RETURN(map->buckets.items != 0, 0);
  HASHY_ASSERT_RETURN(map->buckets.length > 0, 0);

  HashyHash bhash = {0};
  return find_bucketi(map, key, &bhash, false, true);
}

int hashy_map_get_keys(HashyMap* map, HashyKeyList* out) {
  if (!map || !out) return 0;
  if (!out->initialized) {
    hashy_key_list_init(out, map->config.capacity / 2);
  }
  if (map->buckets.length <= 0) return 0;

  for (int64_t i = 0; i < map->buckets.length; i++) {
    HashyBucket bucket = map->buckets.items[i];

    if (bucket.is_set) {
      hashy_key_list_push(out, bucket.key);
    }

    /*
    if (bucket.map != 0) {
      hashy_map_get_keys(bucket.map, out);
      }*/
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

  HashyString key = it->keys.items[it->i];

  it->bucket = hashy_map_get_bucket(map, key.value);

  it->i++;

  if (it->bucket == 0) {
    hashy_key_list_clear(&it->keys);
  }

  return it->bucket != 0;
}
