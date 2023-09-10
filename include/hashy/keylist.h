#ifndef HASHY_KEY_LIST_H
#define HASHY_KEY_LIST_H
#include <stdint.h>
#include <stdbool.h>
#include <hashy/string.h>

typedef struct {
  HashyString* items;
  int64_t avail;
  int64_t capacity;
  int64_t length;
  bool initialized;
} HashyKeyList;

void hashy_key_list_init(HashyKeyList* list, int64_t capacity);
int hashy_key_list_grow(HashyKeyList* list);
int hashy_key_list_push(HashyKeyList* list, HashyString hstr);
int hashy_key_list_remove(HashyKeyList* list, const char* key);
void hashy_key_list_clear(HashyKeyList* list);

#endif
