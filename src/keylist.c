#include <hashy/keylist.h>
#include <hashy/macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hashy_key_list_init(HashyKeyList* list, int64_t capacity) {
  if (!list) return;
  if (list->initialized) return;
  list->initialized = true;
  list->capacity = 0;
  list->avail = 0;
  list->items = 0;
  list->length = 0;
  list->capacity = OR(capacity, 256);
}
int hashy_key_list_grow(HashyKeyList* list) {
  if (!list) return 0;
  if (!list->initialized) HASHY_WARNING_RETURN(0, stderr,  "not initialized.\n");
  if (list->capacity <= 0) HASHY_WARNING_RETURN(0, stderr, "No capacity.\n");
  list->avail += list->capacity;
  list->items = (char**)realloc(list->items, (list->length + list->avail) * sizeof(char*));
  return list->avail > 0 && list->items != 0;
}
int hashy_key_list_push(HashyKeyList* list, const char* key) {
  if (!list) return 0;
  if (!list->initialized) HASHY_WARNING_RETURN(0, stderr,  "not initialized.\n");
  if (!key) HASHY_WARNING_RETURN(0, stderr, "key is null.\n");

  if (list->avail <= 0) {
    if (!hashy_key_list_grow(list)) HASHY_WARNING_RETURN(0, stderr, "failed to grow list.\n");
  }

  char* next_key = strdup(key);
  list->items[list->length++] = next_key;
  list->avail = MAX(0, list->avail - 1);


  return 1;
}

void hashy_key_list_clear(HashyKeyList* list) {
  if (!list) return;


  if (list->items != 0) {
    for (int64_t i = 0; i < list->length; i++) {
      if (list->items[i] != 0) {
        free(list->items[i]);
        list->items[i] = 0;
      }
    }
  }

  if (list->items != 0) {
    free(list->items);
    list->items = 0;
  }

  list->length = 0;
  list->initialized = false;
  hashy_key_list_init(list, list->capacity);
}
