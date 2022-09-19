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
  buffer->avail = 0;
  buffer->used = 0;
  buffer->capacity = OR(capacity, HASHY_DEFAULT_CAPACITY);
}

int hashy_bucket_buffer_grow(HashyBucketBuffer* buffer, int64_t length) {
  if (!buffer) return 0;
  if (!length) return 0;
  if (!buffer->initialized) return 0;

  buffer->items = (HashyBucket*)realloc(buffer->items, (buffer->avail + length) * sizeof(HashyBucket));
  buffer->avail += length;

  return buffer->items != 0 && buffer->avail >= 0;
}


void hashy_bucket_buffer_clear(HashyBucketBuffer* buffer) {
  if (!buffer) return;
  if (!buffer->items) return;

  free(buffer->items);
  buffer->items = 0;
  buffer->avail = 0;
  buffer->used = 0;
}

HashyBucket* hashy_bucket_buffer_set(HashyBucketBuffer* buffer, HashyBucket bucket, uint64_t index) {
  if (!buffer) return 0;
  if (!buffer->initialized) HASHY_WARNING_RETURN(0, stderr, "buffer not initialized.\n");
  if (index < 0) HASHY_WARNING_RETURN(0, stderr, "Index < 0");

  if (!buffer->items) {
    if (!hashy_bucket_buffer_grow(buffer, buffer->capacity)) return 0;
  }

  if (index >= buffer->capacity) HASHY_WARNING_RETURN(0, stderr, "index out of bounds.\n");

  buffer->items[index] = bucket;
  hashy_bucket_init(&buffer->items[index]);

  buffer->used += 1;
  buffer->avail = MAX(0, buffer->avail-1);

  return &buffer->items[index];
}

HashyBucket* hashy_bucket_buffer_get(HashyBucketBuffer* buffer, uint64_t index) {
  if (!buffer) HASHY_WARNING_RETURN(0, stderr, "buffer is null");
  if (index < 0) HASHY_WARNING_RETURN(0, stderr, "Index < 0");
  if (!buffer->initialized) HASHY_WARNING_RETURN(0, stderr, "buffer is not initialized.\n");
  if (!buffer->items) HASHY_WARNING_RETURN(0, stderr, "buffer has no items.\n");
  if (buffer->capacity <= 0) HASHY_WARNING_RETURN(0, stderr, "buffer->capacity <= 0\n");
  if (index >= buffer->capacity) HASHY_WARNING_RETURN(0, stderr, "index >= buffer->capacity");
  return &buffer->items[index];
}
