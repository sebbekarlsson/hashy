#include <hashy/bucket.h>
#include <hashy/hashy.h>
#include <hashy/macros.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void hashy_bucket_init(HashyBucket* bucket) {
  if (!bucket) return;
  if (bucket->initialized) return;
  bucket->initialized = true;
  bucket->key = 0;
  bucket->map = 0;
  bucket->value = 0;
}

// buffer
void hashy_bucket_buffer_init(HashyBucketBuffer* buffer, int64_t capacity) {
  if (!buffer) return;
  if (buffer->initialized) return;
  buffer->initialized = true;
  buffer->items = 0;
  buffer->length = 0;
  buffer->capacity = OR(capacity, HASHY_DEFAULT_CAPACITY);
}

int hashy_bucket_buffer_grow(HashyBucketBuffer* buffer, int64_t length) {
  if (!buffer) return 0;
  if (!length) return 0;
  if (!buffer->initialized) return 0;

  int64_t start = buffer->length;
  buffer->length += length;

  buffer->items = (HashyBucket*)realloc(buffer->items, (buffer->length) * sizeof(HashyBucket));
  if (buffer->items == 0) {
    HASHY_WARNING_RETURN(0, stderr, "Failed to reallocate buffer.\n");
  }
  memset(&buffer->items[start], 0, sizeof(HashyBucket) * (length));

  return buffer->items != 0 && buffer->length >= 0;
}


void hashy_bucket_buffer_clear(HashyBucketBuffer* buffer, bool free_values) {
  if (!buffer) return;

  for (int64_t i = 0; i < buffer->length; i++) {
    hashy_bucket_clear(&buffer->items[i], free_values);
  }

  if (buffer->items) {
    free(buffer->items);
  }
  buffer->items = 0;
  buffer->length = 0;
}

HashyBucket* hashy_bucket_buffer_get(HashyBucketBuffer* buffer, uint64_t index) {
  if (!buffer) HASHY_WARNING_RETURN(0, stderr, "buffer is null");
  if (!buffer->initialized) HASHY_WARNING_RETURN(0, stderr, "buffer is not initialized.\n");
  if (!buffer->items || buffer->length <= 0) HASHY_WARNING_RETURN(0, stderr, "buffer has no items.\n");
  if (index >= buffer->capacity) HASHY_WARNING_RETURN(0, stderr, "index >= buffer->capacity");
  return &buffer->items[index];
}

int hashy_bucket_clear(HashyBucket* bucket, bool free_values) {
  if (!bucket) return 0;
  if (bucket->map != 0) {
    hashy_map_clear(bucket->map, free_values);
    free(bucket->map);
  }

  bucket->map = 0;

  if (bucket->key) {
    free(bucket->key);
  }

  bucket->key = 0;

  if (bucket->value != 0 && free_values == true) {
    free(bucket->value);
  }

  bucket->value = 0;

  return 1;
}
