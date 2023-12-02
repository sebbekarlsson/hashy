#include <bench/mock_data.h>
#include <hashy/hashy.h>
#include <stdio.h>
#include <time.h>

#define BENCH_CAPACITY 4048

#define MEASURE_TIME(name, body)                                               \
  {                                                                            \
    clock_t time_before;                                                       \
    clock_t time_after;                                                        \
    double ms = 0.0;                                                           \
                                                                               \
    time_before = clock();                                                     \
                                                                               \
    {body}                                                                     \
                                                                               \
    time_after = clock();                                                      \
                                                                               \
    ms = (time_after - time_before) / ((double)CLOCKS_PER_SEC) * 1000.0;       \
                                                                               \
    printf("(%s): %12.6f ms\n", name, ms);                                     \
  }

int main(int argc, char *argv[]) {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = BENCH_CAPACITY });

  printf("inserting: %d\n", BENCH_MOCK_DATA_LENGTH);

  MEASURE_TIME("hashy_map_set",
    for (int i = 0; i < BENCH_MOCK_DATA_LENGTH; i++) {
      BenchMockSample sample = samples[i];
      const char* key = sample.key;
      const char* value = sample.value;
      hashy_map_set(&map, key, (void*)value);
    }
  );

  printf("num_collisions: %ld\n", map.num_collisions);
  
  #if 1
  MEASURE_TIME("hashy_map_set (duplicate)",
    for (int i = 0; i < BENCH_MOCK_DATA_LENGTH * 2; i++) {
      BenchMockSample sample = samples[i % BENCH_MOCK_DATA_LENGTH];
      const char* key = sample.key;
      const char* value = sample.value;
      hashy_map_set(&map, key, (void*)value);
    }
  );
  #endif
  hashy_map_destroy(&map);
  hashy_map_clear(&map);
}
