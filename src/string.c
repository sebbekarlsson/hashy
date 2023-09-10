#include <hashy/string.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void hashy_string_clear(HashyString* hstr) {
  if (!hstr) return;
  memset(&hstr->value[0], 0, HASHY_MAX_KEY_LENGTH * sizeof(char));
  hstr->length = 0;
}

void hashy_string_copy(HashyString* dest, HashyString src) {
  if (dest == 0) return;
  hashy_string_clear(dest);
  snprintf(&dest->value[0], HASHY_MAX_KEY_LENGTH-1, "%s", src.value);
}

HashyString hashy_string_make(const char* value) {
  if (!value) return (HashyString){0};

  HashyString s = {0};
  snprintf(&s.value[0], HASHY_MAX_KEY_LENGTH-1, "%s", value);
  return s;
}
