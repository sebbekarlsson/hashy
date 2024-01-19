#include <hashy/lock.h>
#include <hashy/macros.h>
#include <stdlib.h>
#include <stdio.h>

int hashy_mutex_init(HashyMutex* mutex) {
  HASHY_ASSERT_RETURN(mutex != 0, 0);
  if (mutex->initialized) return 1;

  if (pthread_mutexattr_init(&mutex->attr)) {
    HASHY_WARNING_RETURN(0, stderr, "Failed to initialize mutex attribute.\n");
  }

  if (pthread_mutexattr_settype(&mutex->attr, PTHREAD_MUTEX_RECURSIVE)) {
    HASHY_WARNING_RETURN(0, stderr, "Failed to set mutexattr PTHREAD_MUTEX_RECURSIVE.\n");
  }

  if (pthread_mutex_init(&mutex->lock, &mutex->attr)) {
    HASHY_WARNING_RETURN(0, stderr, "Failed to initialize mutex.\n");
  }

  mutex->initialized = true;
  return 1;
}
int hashy_mutex_lock(HashyMutex* mutex) {
  HASHY_ASSERT_RETURN(mutex != 0, 0);
  HASHY_ASSERT_RETURN(mutex->initialized == true, 0);
  pthread_mutex_lock(&mutex->lock);
  return 1;
}

int hashy_mutex_unlock(HashyMutex* mutex) {
  HASHY_ASSERT_RETURN(mutex != 0, 0);
  HASHY_ASSERT_RETURN(mutex->initialized == true, 0);
  pthread_mutex_unlock(&mutex->lock);
  return 1;
}

int hashy_mutex_destroy(HashyMutex* mutex) {
  HASHY_ASSERT_RETURN(mutex != 0, 0);
  HASHY_ASSERT_RETURN(mutex->initialized == true, 0);
  pthread_mutex_destroy(&mutex->lock);
  pthread_mutexattr_destroy(&mutex->attr);
  mutex->initialized = false;
  return 1;
}
