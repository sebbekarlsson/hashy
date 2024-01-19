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
  bool free_linked_on_clear;
  bool threadsafe;
} HashyConfig;
#endif
