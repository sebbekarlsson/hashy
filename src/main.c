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
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true });


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
  hashy_map_init(&map, (HashyConfig){ .capacity = 256 });

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


static void test_get_without_set() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 16 });

  void* yo = hashy_map_get(&map, "hello");

  HASHY_TASSERT(yo == 0);

  hashy_map_destroy(&map);
}

static void test_unset_without_values() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 16 });

  hashy_map_unset(&map, "hello");
  void* yo = hashy_map_get(&map, "hello");
  HASHY_TASSERT(yo == 0);
  hashy_map_destroy(&map);
  HASHY_TASSERT(map.buckets.items == 0);
}

static void test_set_clear_and_get() {
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 16, .free_values_on_clear = true, .free_values_on_destroy = true });


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
  int64_t n_vectors = 64;
  HashyMap map = {0};
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true, .free_values_on_clear = true, .free_values_on_overwrite = true, .free_values_on_unset = true, .free_linked_on_clear = true });

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

  hashy_map_clear(&map);


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
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true });

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
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true });

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
  hashy_map_init(&map, (HashyConfig){ .capacity = 256, .free_values_on_destroy = true, .free_values_on_unset = true });

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


  test_simple();
  test_big();
  test_get_without_set();
  test_unset_without_values();
  test_set_clear_and_get();
  test_set_vectors();
  //test_set_same();
  //test_get_keys();
  //test_iterate();

  printf("OK, all tests passed.\n");

  return 0;
}
