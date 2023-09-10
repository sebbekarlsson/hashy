#ifndef HASHY_STRING_H
#define HASHY_STRING_H
#include <hashy/constants.h>
#include <stdint.h>

typedef struct {
  char value[HASHY_MAX_KEY_LENGTH];
  int64_t length;
} HashyString;

void hashy_string_clear(HashyString* hstr);

void hashy_string_copy(HashyString* dest, HashyString src);

HashyString hashy_string_make(const char* value);

#endif
