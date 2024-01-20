#include <hashy/hashy.h>
#include <hashy/macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define HASHY_TASSERT(expr) { if (!(expr)) { fprintf(stderr, "FAIL: (%s)(%s)\n", #expr, __func__); assert(expr); } else if (0) {  printf("OK: (%s)(%s).\n", #expr, __func__); } }


typedef struct {
  int age;
} Person;


static void test_simple() {

  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true, .threadsafe = true });


  Person* p = calloc(1, sizeof(Person));
  p->age = 33;
  Person* p2 = calloc(1, sizeof(Person));
  p2->age = 64;

  HASHY_TASSERT(hashy_map_set(&map, "john", p) != 0);
  HASHY_TASSERT(hashy_map_set(&map, "sarah", p2) != 0);


  Person* a = hashy_map_get(&map, "john");
  Person* b = hashy_map_get(&map, "sarah");

  HASHY_TASSERT(a != 0);
  HASHY_TASSERT(b != 0);

  HASHY_TASSERT(a->age == 33);
  HASHY_TASSERT(b->age == 64);
  hashy_map_destroy(&map);
}

static void test_simple_i() {

  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true, .threadsafe = true });


  Person* p = calloc(1, sizeof(Person));
  p->age = 33;
  Person* p2 = calloc(1, sizeof(Person));
  p2->age = 64;

  HashyI642 key_0 = {3, 3124};
  HashyI642 key_1 = {1, 7};

  HASHY_TASSERT(hashy_map_seti(&map, key_0, p) != 0);
  HASHY_TASSERT(hashy_map_seti(&map, key_1, p2) != 0);


  Person* a = hashy_map_geti(&map, key_0);
  Person* b = hashy_map_geti(&map, key_1);

  HASHY_TASSERT(a != 0);
  HASHY_TASSERT(b != 0);

  HASHY_TASSERT(a->age == 33);
  HASHY_TASSERT(b->age == 64);
  hashy_map_destroy(&map);
}

static void test_big() {

    const char* keys[] = { "contextually-based",
                         "bi-directional",
                         "access",
                         "concept",
                         "Fundamental",
                         "5th generation",
                         "Reactive",
                         "productivity",
                         "hybrid",
                         "impactful",
                         "discrete",
                         "User-friendly",
                         "scalable",
                         "Customer-focused",
                         "info-mediaries",
                         "homogeneous",
                         "foreground",
                         "focus group",
                         "intangible",
                         "mission-critical",
                         "paradigm",
                         "client-driven",
                         "orchestration",
                         "algorithm",
                         "needs-based",
                         "Implemented",
                         "Exclusive",
                         "asymmetric",
                         "Profit-focused",
                         "ability",
                         "strategy",
                         "portal",
                         "hierarchy",
                         "collaboration",
                         "core",
                         "Open-architected",
                         "Fully-configurable",
                         "moderator",
                         "transitional",
                         "incremental",
                         "Centralized",
                         "Optimized",
                         "Universal",
                         "Progressive",
                         "Polarised",
                         "composite",
                         "client-server",
                         "projection",
                         "full-range",
                         "data-warehouse",
                         "Cross-platform",
                         "zero defect",
                         "product",
                         "regional",
                         "superstructure",
                         "Streamlined",
                         "ability",
                         "Optional",
                         "alliance",
                         "service-desk",
                         "De-engineered",
                         "knowledge base",
                         "asymmetric",
                         "demand-driven",
                         "Integrated",
                         "object-oriented",
                         "policy",
                         "local area network",
                         "3rd generation",
                         "Multi-lateral",
                         "De-engineered",
                         "3rd generation",
                         "optimal",
                         "upward-trending",
                         "grid-enabled",
                         "installation",
                         "Virtual",
                         "client-driven",
                         "executive",
                         "systemic",
                         "Function-based",
                         "groupware",
                         "website",
                         "neural-net",
                         "Re-contextualized",
                         "Future-proofed",
                         "Monitored",
                         "structure",
                         "matrix",
                         "infrastructure",
                         "Streamlined" };

  const char* values[] = {
    "Ophioglossaceae",  "Cyperaceae",    "Chenopodiaceae",   "Poaceae",          "Convolvulaceae",
    "Nymphaeaceae",     "Moraceae",      "Scrophulariaceae", "Opegraphaceae",    "Fabaceae",
    "Rhamnaceae",       "Fabaceae",      "Rubiaceae",        "Fabaceae",         "Fabaceae",
    "Teloschistaceae",  "Cucurbitaceae", "Apocynaceae",      "Scrophulariaceae", "Pteridaceae",
    "Dicranaceae",      "Poaceae",       "Campanulaceae",    "Teloschistaceae",  "Ranunculaceae",
    "Rubiaceae",        "Fabaceae",      "Cyperaceae",       "Fabaceae",         "Roccellaceae",
    "Aristolochiaceae", "Acanthaceae",   "Cactaceae",        "Myrtaceae",        "Adoxaceae",
    "Cyperaceae",       "Apocynaceae",   "Chenopodiaceae",   "Euphorbiaceae",    "Fabaceae",
    "Salicaceae",       "Asteraceae",    "Apiaceae",         "Lecanoraceae",     "Rosaceae",
    "Vitaceae",         "Thymelaeaceae", "Primulaceae",      "Liliaceae",        "Scrophulariaceae",
    "Polygonaceae",     "Violaceae",     "Verbenaceae",      "Poaceae",          "Lamiaceae"
  };

  int64_t nrkeys = sizeof(keys) / sizeof(keys[0]);
  int64_t nrvalues = sizeof(values) / sizeof(values[0]);




  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .threadsafe = true });

  for (int64_t i = 0; i < nrkeys; i++) {
    const char* key = keys[i];
    const char* expected = values[i % nrvalues];
    char* value = strdup(values[i % nrvalues]);

    HASHY_TASSERT(hashy_map_set(&map, key, value) != 0);

    HashyBucket* bucket = hashy_map_get_bucket(&map, key);

    HASHY_TASSERT(bucket != 0);

    HASHY_TASSERT(strcmp(bucket->key.value, key) == 0);

    char* back_value = hashy_map_get(&map, key);

    HASHY_TASSERT(back_value != 0);
    HASHY_TASSERT(strcmp(back_value, expected) == 0);

    hashy_map_unset(&map, key);

    free(back_value);
    back_value = 0;
  } 

  hashy_map_destroy(&map);
}

static void test_mega_big() {


  int64_t nrKeys = 50000;
  int64_t nrValues = 70000;


  uint32_t* keys = calloc(nrKeys, sizeof(uint32_t));
  uint32_t* values = calloc(nrValues, sizeof(uint32_t));

  const char* words[] = {
    "apple",
    "pear",
    "banana",
    "orange"
  };
  
  int64_t nrWords = sizeof(words) / sizeof(words[0]);

  for (int64_t i = 0; i < nrKeys; i++) {
    uint32_t n = i + (~i >> 3U);
    n ^= n << 17U;
    uint32_t k = ~n;
    n ^= n >> 13U;
    n ^= n << 5U;
    n += (n * 50391U + k * 71U);
    n *= 1013U;
    keys[i] = n;
  }

  for (int64_t i = 0; i < nrValues; i++) {
    uint32_t n = 10391U+33u*(i + (~i >> 3U));
    n ^= n << 17U;
    uint32_t k = ~n;
    n ^= n >> 13U;
    n ^= n << 5U;
    n += (n * 72091U + k * 11U);
    n *= 5013U;
    values[i] = n;
  }


  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .threadsafe = true });

  for (int64_t i = 0; i < nrValues; i++) {
    uint32_t keyi = keys[i % nrKeys];
    uint32_t expectedi = values[i % nrValues];

    int64_t a = keyi;
    int64_t b = expectedi;

    const char* wa = words[a % nrWords];
    const char* wb = words[b % nrWords];
    
    char key[512] = {0};
    snprintf(key, 512-1, "%s-%d", wa, keyi);
    char value[512] = {0};
    snprintf(value, 512-1, "%s-%d", wb, expectedi);

    // printf("%s -> %s\n", key, value);
    HASHY_TASSERT(hashy_map_set(&map, key, value) != 0);

    HashyBucket* bucket = hashy_map_get_bucket(&map, key);

    HASHY_TASSERT(bucket != 0);

    HASHY_TASSERT(strcmp(bucket->key.value, key) == 0);

    char* back_value = hashy_map_get(&map, key);

    HASHY_TASSERT(back_value != 0);
    HASHY_TASSERT(strcmp(back_value, value) == 0);

    //hashy_map_unset(&map, key);

    back_value = 0;
  }

  hashy_map_clear(&map);
  printf("mega:\n");
  printf("Collisions: %ld\n", map.num_collisions);
  printf("Pages: %ld\n", map.num_pages);

  hashy_map_destroy(&map);


  free(keys);
  free(values);

  keys = 0;
  values = 0;
}

static void test_big_i() {

  int64_t keys[] = {11, 148, 11, 33, 24, 244, 14, 182, 94, 171, 92, 90, 71, 241, 28, 239, 183, 179, 224, 102, 132, 75, 252, 78, 87, 106, 155, 122, 119, 204, 143, 162, 108, 174, 189, 68, 60, 161, 105, 12, 66, 164, 181, 1, 225, 168, 9, 51, 112, 243, 88, 149, 173, 17, 50, 114, 89, 222, 79, 249, 139, 45, 124, 242, 113, 118, 150, 232, 206, 135, 83, 134, 15, 85, 193, 212, 95, 234, 194, 123, 185, 48, 126, 248, 210, 37, 226, 233, 223, 61, 125, 180, 221, 220, 96, 253, 169, 23, 170, 216, 21, 137, 30, 172, 195, 5, 41, 63, 165, 128, 158, 3, 18, 231, 31, 72, 238, 107, 196, 209, 57, 97, 25, 56, 32, 27, 205, 111, 101, 167, 103, 120, 115, 26, 199, 82, 236, 246, 227, 70, 62, 47, 184, 6, 91, 136, 58, 175, 138, 251, 229, 67, 131, 142, 213, 177, 110, 151, 65, 197, 39, 133, 144, 218, 254, 230, 198, 127, 129, 13, 76, 176, 141, 8, 104, 109, 215,
153, 235, 191, 217, 237, 69, 36, 159, 54, 46, 203, 55, 154, 34, 38, 130, 250, 200, 192, 7, 201, 64, 2, 98, 152, 190, 4, 207, 240, 86, 59, 74, 187, 156, 214, 145, 146, 42, 208, 116, 247, 20, 19, 99, 81, 245, 166, 93, 163, 228, 211, 255, 44, 49, 22, 29, 178, 0, 100, 53, 202, 117, 121, 157, 84, 77, 40, 160, 10, 140, 35, 80, 52, 186, 16, 219, 43, 188, 147};

  const char* values[] = {
    "Ophioglossaceae",  "Cyperaceae",    "Chenopodiaceae",   "Poaceae",          "Convolvulaceae",
    "Nymphaeaceae",     "Moraceae",      "Scrophulariaceae", "Opegraphaceae",    "Fabaceae",
    "Rhamnaceae",       "Fabaceae",      "Rubiaceae",        "Fabaceae",         "Fabaceae",
    "Teloschistaceae",  "Cucurbitaceae", "Apocynaceae",      "Scrophulariaceae", "Pteridaceae",
    "Dicranaceae",      "Poaceae",       "Campanulaceae",    "Teloschistaceae",  "Ranunculaceae",
    "Rubiaceae",        "Fabaceae",      "Cyperaceae",       "Fabaceae",         "Roccellaceae",
    "Aristolochiaceae", "Acanthaceae",   "Cactaceae",        "Myrtaceae",        "Adoxaceae",
    "Cyperaceae",       "Apocynaceae",   "Chenopodiaceae",   "Euphorbiaceae",    "Fabaceae",
    "Salicaceae",       "Asteraceae",    "Apiaceae",         "Lecanoraceae",     "Rosaceae",
    "Vitaceae",         "Thymelaeaceae", "Primulaceae",      "Liliaceae",        "Scrophulariaceae",
    "Polygonaceae",     "Violaceae",     "Verbenaceae",      "Poaceae",          "Lamiaceae"
  };

  int64_t nrkeys = sizeof(keys) / sizeof(keys[0]);
  int64_t nrvalues = sizeof(values) / sizeof(values[0]);




  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = nrkeys, .threadsafe = true });

  for (int64_t i = 0; i < nrkeys*9; i++) {
    int64_t key_a = keys[i % nrkeys];
    const char* expected = values[i % nrvalues];
    char* value = strdup(values[i % nrvalues]);

    HashyI642 key = (HashyI642){ key_a, keys[key_a % nrkeys] };

    for (int j = 0; j < 8; j++) {
      HASHY_TASSERT(hashy_map_seti(&map, key, value) != 0);
    }

    HashyBucket* bucket = hashy_map_get_bucketi(&map, key);

    HASHY_TASSERT(bucket != 0);

    HASHY_TASSERT(bucket->pair.a == key.a);
    HASHY_TASSERT(bucket->pair.b == key.b);

    char* back_value = hashy_map_geti(&map, key);

    //  printf("(%ld, %ld): %ld\n", bucket->pair.a, bucket->pair.b, bucket->hash);

    HASHY_TASSERT(back_value != 0);
    HASHY_TASSERT(strcmp(back_value, expected) == 0);

    if (i > nrkeys / 2 && i % 2 == 0) {
      hashy_map_unseti(&map, key);
    }

    free(back_value);
    back_value = 0;
  } 

  printf("Collisions: %ld\n", map.num_collisions);
  printf("Pages: %ld\n", map.num_pages);
  hashy_map_destroy(&map);
}


static void test_get_without_set() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 16, .threadsafe = true });

  void* yo = hashy_map_get(&map, "hello");

  HASHY_TASSERT(yo == 0);

  hashy_map_destroy(&map);
}

static void test_get_without_set_i() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 16, .threadsafe = true });

  HashyI642 key = { 311, 2 };
  void* yo = hashy_map_geti(&map, key);

  HASHY_TASSERT(yo == 0);

  hashy_map_destroy(&map);
}

static void test_unset_without_values() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 16, .threadsafe = true });

  hashy_map_unset(&map, "hello");
  void* yo = hashy_map_get(&map, "hello");
  HASHY_TASSERT(yo == 0);
  hashy_map_destroy(&map);
  HASHY_TASSERT(map.buckets.items == 0);
}

static void test_unset_without_values_i() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 16, .threadsafe = true});

  HashyI642 key = {4828, 7};
  hashy_map_unseti(&map, key);
  void* yo = hashy_map_geti(&map, key);
  HASHY_TASSERT(yo == 0);
  hashy_map_destroy(&map);
  HASHY_TASSERT(map.buckets.items == 0);
}

static void test_set_clear_and_get() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 16, .free_values_on_clear = true, .free_values_on_destroy = true, .threadsafe = true });


  const char* pname = "John Doe";
  char* name = strdup(pname);
  hashy_map_set(&map, "name", name);

  char* value = hashy_map_get(&map, "name");

  HASHY_TASSERT(value != 0);
  HASHY_TASSERT(strcmp(value, pname) == 0);

  hashy_map_unset(&map, "name");

  value = hashy_map_get(&map, "name");

  HASHY_TASSERT(value == 0);

  hashy_map_set(&map, "name", name);
  value = hashy_map_get(&map, "name");
  
  HASHY_TASSERT(value != 0);
  HASHY_TASSERT(strcmp(value, pname) == 0);

  hashy_map_clear(&map);

  value = hashy_map_get(&map, "name");

  HASHY_TASSERT(value == 0);

  hashy_map_destroy(&map);
}

static void test_set_clear_and_get_i() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 16, .free_values_on_clear = true, .free_values_on_destroy = true, .threadsafe = true });


  HashyI642 key = {441, 84};
  const char* pname = "John Doe";
  char* name = strdup(pname);
  hashy_map_seti(&map, key, name);

  char* value = hashy_map_geti(&map, key);

  HASHY_TASSERT(value != 0);
  HASHY_TASSERT(strcmp(value, pname) == 0);

  hashy_map_unseti(&map, key);

  value = hashy_map_geti(&map, key);

  HASHY_TASSERT(value == 0);

  hashy_map_seti(&map, key, name);
  value = hashy_map_geti(&map, key);
  
  HASHY_TASSERT(value != 0);
  HASHY_TASSERT(strcmp(value, pname) == 0);

  hashy_map_clear(&map);

  value = hashy_map_geti(&map, key);

  HASHY_TASSERT(value == 0);

  hashy_map_destroy(&map);
}

typedef struct {
  float x;
  float y;
  float z;
} HashyVector;


//static float hashy_float_rand(float min, float max)
//{
//    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
 //   return min + scale * ( max - min );      /* [min, max] */
//}

static void test_set_vectors() {
  int64_t n_vectors = 640;
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true, .free_values_on_clear = true, .free_values_on_overwrite = true, .free_values_on_unset = true, .free_linked_on_clear = true, .threadsafe = true });

  for (int j = 0; j < 16; j++) {
    for (int64_t i = 0; i < n_vectors; i++) {
      float x = (float)j + 0.01f;//hashy_float_rand(-1.0f, 1.0f);
      float y = (float)i+j + 0.5f;//hashy_float_rand(-1.0f, 1.0f);
      float z = (float)(x+y) / 3.14f;//hashy_float_rand(-1.0f, 1.0f);

      HashyVector* v = NEW(HashyVector);
      v->x = x;
      v->y = y;
      v->z = z;

      char tmp[256];
      sprintf(tmp, "%1.3f;%1.3f;%1.3f", v->x, v->y, v->z);


      HASHY_TASSERT(hashy_map_set(&map, tmp, v) != 0);


      HashyBucket* bucket = hashy_map_get_bucket(&map, tmp);

      HASHY_TASSERT(bucket != 0);
      HASHY_TASSERT(strcmp(bucket->key.value, tmp) == 0);


      HashyVector* vv = hashy_map_get(&map, tmp);

      HASHY_TASSERT(vv  != 0);

      HASHY_TASSERT(vv->x == x);
      HASHY_TASSERT(vv->y == y);
      HASHY_TASSERT(vv->z == z);
    }
  }

  for (int j = 0; j < 16; j++) {
    for (int64_t i = 0; i < n_vectors; i++) {
      float x = (float)j + 0.01f;//hashy_float_rand(-1.0f, 1.0f);
      float y = (float)i+j + 0.5f;//hashy_float_rand(-1.0f, 1.0f);
      float z = (float)(x+y) / 3.14f;//hashy_float_rand(-1.0f, 1.0f);

      char tmp[256];
      sprintf(tmp, "%1.3f;%1.3f;%1.3f", x, y, z);



      HashyBucket* bucket = hashy_map_get_bucket(&map, tmp);

      HASHY_TASSERT(bucket != 0);
      HASHY_TASSERT(strcmp(bucket->key.value, tmp) == 0);

      HashyVector* vv = hashy_map_get(&map, tmp);

      HASHY_TASSERT(vv->x == x);
      HASHY_TASSERT(vv->y == y);
      HASHY_TASSERT(vv->z == z);
    }
  }

  int64_t counted = hashy_map_count_pages(&map);
  int64_t cached_counted = map.num_pages;

  printf("counted: %ld, cached counted: %ld\n", counted, cached_counted);
  HASHY_TASSERT(counted == cached_counted);

  hashy_map_clear(&map);

  counted = hashy_map_count_pages(&map);
  HASHY_TASSERT(counted == 0);
  cached_counted = map.num_pages;
  printf("counted: %ld, cached counted: %ld\n", counted, cached_counted);
  HASHY_TASSERT(cached_counted == 0);
  HASHY_TASSERT(counted == cached_counted);


  for (int j = 0; j < 16; j++) {
    for (int64_t i = 0; i < n_vectors; i++) {
      float x = (float)j + 0.01f;//hashy_float_rand(-1.0f, 1.0f);
      float y = (float)i+j + 0.5f;//hashy_float_rand(-1.0f, 1.0f);
      float z = (float)(x+y) / 3.14f;//hashy_float_rand(-1.0f, 1.0f);

      HashyVector* v = NEW(HashyVector);
      v->x = x;
      v->y = y;
      v->z = z;

      char tmp[256];
      sprintf(tmp, "%1.3f;%1.3f;%1.3f", v->x, v->y, v->z);


      HASHY_TASSERT(hashy_map_set(&map, tmp, v) != 0);


      HashyBucket* bucket = hashy_map_get_bucket(&map, tmp);

      HASHY_TASSERT(bucket != 0);
      HASHY_TASSERT(strcmp(bucket->key.value, tmp) == 0);


      HashyVector* vv = hashy_map_get(&map, tmp);

      HASHY_TASSERT(vv  != 0);

      HASHY_TASSERT(vv->x == x);
      HASHY_TASSERT(vv->y == y);
      HASHY_TASSERT(vv->z == z);
    }
  }
  

  hashy_map_destroy(&map);
}


static void test_set_same() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true, .threadsafe = true });

  const char* message = "hello world";
  char* mystr = strdup(message);

  int64_t iter = 100;
  const char* mykey = "message";

  for (int64_t i = 0; i < 100; i++) {
    hashy_map_set(&map, mykey, mystr);
  }

  for (int64_t i = 0; i < 100; i++) {
    char* value = hashy_map_get(&map, mykey);

    HASHY_TASSERT(value != 0);
    HASHY_TASSERT(strcmp(value, message) == 0);
  }

   for (int64_t i = 0; i < 100; i++) {
    hashy_map_set(&map, mykey, mystr);
  }

  for (int64_t i = 0; i < 100; i++) {
    char* value = hashy_map_get(&map, mykey);

    HASHY_TASSERT(value != 0);
    HASHY_TASSERT(strcmp(value, message) == 0);
  }



  hashy_map_destroy(&map);
}

static void test_get_keys() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true, .threadsafe = true });

  int64_t nr_items = 125;



  for (int64_t i = 0; i < nr_items; i++) {
    char tmp[256];
    sprintf(tmp, "item_%ld", i);
    char* x = strdup("123");
    hashy_map_set(&map, tmp, x);
  }



  HashyKeyList list = {0};
  hashy_map_get_keys(&map, &list);

  HASHY_TASSERT(list.length == nr_items);
  HASHY_TASSERT(list.items != 0);

  hashy_key_list_clear(&list);

  HASHY_TASSERT(list.items == 0);
  HASHY_TASSERT(list.length == 0);
  HASHY_TASSERT(list.avail == 0);
  HASHY_TASSERT(list.length == 0);

  hashy_map_destroy(&map);
}

static void test_iterate() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true, .free_values_on_unset = true, .threadsafe = true });

  int64_t nr_items = 125;



  for (int64_t i = 0; i < nr_items; i++) {
    char tmp[256];
    sprintf(tmp, "item_%ld", i);
    char* x = strdup(tmp);
    hashy_map_set(&map, tmp, x);
  }

  HashyIterator it = {0};

  int64_t iter_count = 0;
  while (hashy_map_iterate(&map, &it)) {
    char* key = it.bucket->value;
    iter_count++;
  }

  HASHY_TASSERT(iter_count == nr_items);
  HASHY_TASSERT(it.keys.length <= 0);
  HASHY_TASSERT(it.keys.items == 0);

  HASHY_TASSERT(hashy_map_unset(&map, "item_0") == 1);

  hashy_map_destroy(&map);
}

int main(int argc, char* argv[]) {

  srand(time(0));

  test_mega_big();

  //  printf("OK, all tests passed.\n");
  ////return 0;

  test_simple();
  test_big();
  test_mega_big();
  test_get_without_set();
  test_unset_without_values();
  test_set_clear_and_get();
  test_set_vectors();

  test_simple_i();
  test_big_i();
  test_get_without_set_i();
  test_unset_without_values_i();
  test_set_clear_and_get_i();
  //test_set_same();
  //test_get_keys();
  //test_iterate();

  printf("OK, all tests passed.\n");

  return 0;
}
