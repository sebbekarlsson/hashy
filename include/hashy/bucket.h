#ifndef HASHY_BUCKET_H
#define HASHY_BUCKET_H
#include <stdint.h>
#include <stdbool.h>

#define HASHY_DEFAULT_CAPACITY 256

struct HASHY_MAP_STRUCT;
struct HASHY_BUCKET_BUFFER;

typedef struct {
  char* key;
  void* value;

  struct HASHY_MAP_STRUCT* map;
  bool initialized;
} HashyBucket;

int hashy_bucket_clear(HashyBucket* bucket, bool free_values);

typedef struct HASHY_BUCKET_BUFFER {
  int64_t length;
  HashyBucket* items;
  bool initialized;
  int64_t capacity;
} HashyBucketBuffer;

void hashy_bucket_init(HashyBucket* bucket);


// buffer
void hashy_bucket_buffer_init(HashyBucketBuffer* buffer, int64_t capacity);

int hashy_bucket_buffer_grow(HashyBucketBuffer* buffer, int64_t length);

void hashy_bucket_buffer_clear(HashyBucketBuffer* buffer, bool free_values);

HashyBucket* hashy_bucket_buffer_get(HashyBucketBuffer* buffer, uint64_t index);

#endif
