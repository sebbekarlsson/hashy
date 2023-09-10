#ifndef HASHY_CONFIG_H
#define HASHY_CONFIG_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  int64_t capacity;
  bool free_values_on_destroy;
  bool free_values_on_unset;
  bool free_values_on_clear;
  bool free_values_on_overwrite;
  bool destroy_recursion_on_clear;
} HashyConfig;
#endif
