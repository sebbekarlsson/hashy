#ifndef HASHY_MACROS_H
#define HASHY_MACROS_H


#ifndef OR
#define OR(a, b) (a ? a : b)
#endif

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#endif


#define HASHY_CLI_RED "\x1B[31m"
#define HASHY_CLI_GREEN "\x1B[32m"
#define HASHY_CLI_YELLLOW "\x1B[33m"
#define HASHY_CLI_BLUE "\x1B[34m"
#define HASHY_CLI_MAGENTA "\x1B[35m"
#define HASHY_CLI_CYAN "\x1B[36m"
#define HASHY_CLI_WHITE "\x1B[37m"
#define HASHY_CLI_RESET "\x1B[0m"

#define HASHY_WARNING(...)                                                       \
  {                                                                            \
    printf(HASHY_CLI_RED "(HASHY)(Warning)(%s:%s:%d): \n" HASHY_CLI_RESET, __FILE__, __func__, \
           __LINE__);                                                          \
    fprintf(__VA_ARGS__);                                                      \
  }

#define HASHY_WARNING_RETURN(ret, ...)                                           \
  {                                                                            \
    printf("\n****\n");                                                        \
    printf(HASHY_CLI_RED "(HASHY)(Warning)(%s:%d): \n" HASHY_CLI_RESET, __func__, __LINE__); \
    fprintf(__VA_ARGS__);                                                      \
    printf("\n****\n");                                                        \
    return ret;                                                                \
  }

#define HASHY_WARNING_RETURN_NOT_INITIALIZED(ret)                                \
  {                                                                            \
    printf("\n****\n");                                                        \
    printf(HASHY_CLI_RED "(HASHY)(Warning)(%s): \n" HASHY_CLI_RESET, __func__);      \
    fprintf(stderr, "Not initialized.\n");                                     \
    printf("\n****\n");                                                        \
    return ret;                                                                \
  }

#define HASHY_PANIC(fp, ...)                                                     \
  {                                                                            \
    HASHY_WARNING(fp, __VA_ARGS__);                                              \
    exit(0);                                                                   \
  }

#define HASHY_ASSERT_PANIC(expr)                                                 \
  {                                                                            \
    if (!(expr)) {                                                             \
      HASHY_PANIC(stderr, "Assertion failed: %s.\n", #expr);                     \
    }                                                                          \
  }


#define HASHY_ASSERT_RETURN(expr, ret)                                           \
  {                                                                            \
    if (!(expr)) {                                                             \
      HASHY_WARNING(stderr, "(HASHY): Assertion failed: %s.\n", #expr);            \
      return ret;                                                              \
    }                                                                          \
  }
#define HASHY_ASSERT_CONTINUE(expr)                                              \
  {                                                                            \
    if (!(expr)) {                                                             \
      HASHY_WARNING(stderr, "(HASHY): Assertion failed: %s.\n", #expr);            \
      continue;                                                                \
    }                                                                          \
  }



#ifndef NEW
#define NEW(T) ((T*)calloc(1, sizeof(T)))
#endif

#endif
