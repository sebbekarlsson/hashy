#ifndef HASHY_KEY_LIST_H
#define HASHY_KEY_LIST_H
#include <stdint.h>
#include <stdbool.h>
typedef struct {
  char** items;
  int64_t used;
  int64_t avail;
  int64_t capacity;
  int64_t length;
  int64_t total_length;
  bool initialized;
} HashyKeyList;

void hashy_key_list_init(HashyKeyList* list, int64_t capacity);
int hashy_key_list_grow(HashyKeyList* list);
int hashy_key_list_push(HashyKeyList* list, const char* key);
void hashy_key_list_clear(HashyKeyList* list);

#endif
