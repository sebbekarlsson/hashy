#ifndef HASHY_THREADING_LOCK_H
#define HASHY_THREADING_LOCK_H
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  pthread_mutex_t lock;
  pthread_mutexattr_t attr;
  bool initialized;
} HashyMutex;

int hashy_mutex_init(HashyMutex* mutex);
int hashy_mutex_lock(HashyMutex* mutex);
int hashy_mutex_unlock(HashyMutex* mutex);
int hashy_mutex_destroy(HashyMutex* mutex);

#endif
