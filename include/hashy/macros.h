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

#define HASHY_WARNING_RETURN(ret, ...)                                          \
  {                                                                            \
    printf("\n****\n");                                                        \
    printf("(HASHY)(Warning)(%s): \n", __func__);   \
    fprintf(__VA_ARGS__);                                                      \
    printf("\n****\n");                                                        \
    return ret;                                                                \
  }


#ifndef NEW
#define NEW(T) ((T*)calloc(1, sizeof(T)))
#endif

#endif
