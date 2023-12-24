#include <hashy/cantor.h>
#include <math.h>

int64_t hashy_cantor(int64_t k1, int64_t k2) {
  return ((k1 + k2) * (k1 + k2 + 1)) / 2 + k2;
}

void hashy_decant(int64_t c, int64_t* x, int64_t* y) {
  int w = floor((sqrt(c * 8 + 1) - 1) / 2);
  *y = c - (w * (w + 1)) / 2;
  *x = w - *y;
}
