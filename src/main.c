#include <hashy/hashy.h>
#include <hashy/macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define HASHY_ASSERT(expr) { if (!(expr)) { fprintf(stderr, "FAIL: (%s)(%s)\n", #expr, __func__); assert(expr); } else {printf("OK: (%s)(%s).\n", #expr, __func__); } }


typedef struct {
  int age;
} Person;


void test_simple() {

  HashyMap map = {0};
  hashy_map_init(&map, 256);


  Person* p = calloc(1, sizeof(Person));
  p->age = 33;
  Person* p2 = calloc(1, sizeof(Person));
  p2->age = 64;

  HASHY_ASSERT(hashy_map_set(&map, "john", p) != 0);
  HASHY_ASSERT(hashy_map_set(&map, "sarah", p2) != 0);


  Person* a = hashy_map_get(&map, "john");
  Person* b = hashy_map_get(&map, "sarah");

  HASHY_ASSERT(a != 0);
  HASHY_ASSERT(b != 0);

  HASHY_ASSERT(a->age == 33);
  HASHY_ASSERT(b->age == 64);
  hashy_map_clear(&map, true);
}

void test_big() {

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
  hashy_map_init(&map, 16);

  for (int64_t i = 0; i < nrkeys; i++) {
    const char* key = keys[i];
    const char* expected = values[i % nrvalues];
    char* value = strdup(values[i % nrvalues]);

    HASHY_ASSERT(hashy_map_set(&map, key, value) != 0);

    HashyBucket* bucket = hashy_map_get_bucket(&map, key);

    HASHY_ASSERT(bucket != 0);

    HASHY_ASSERT(bucket->key != 0);
    HASHY_ASSERT(strcmp(bucket->key, key) == 0);

    char* back_value = hashy_map_get(&map, key);

    HASHY_ASSERT(back_value != 0);
    HASHY_ASSERT(strcmp(back_value, expected) == 0);

    char* to_free = hashy_map_unset(&map, key);
    HASHY_ASSERT(to_free != 0);

    free(to_free);
    to_free = 0;
  }

  hashy_map_clear(&map, true);
}


void test_get_without_set() {
  HashyMap map = {0};
  hashy_map_init(&map, 16);

  void* yo = hashy_map_get(&map, "hello");

  HASHY_ASSERT(yo == 0);
}

void test_unset_without_values() {
  HashyMap map = {0};
  hashy_map_init(&map, 16);

  void* yo = hashy_map_unset(&map, "hello");
  HASHY_ASSERT(yo == 0);
  hashy_map_clear(&map, true);
  HASHY_ASSERT(map.buckets.items == 0);
}

void test_set_clear_and_get() {
  HashyMap map = {0};
  hashy_map_init(&map, 16);


  const char* pname = "John Doe";
  char* name = strdup(pname);
  hashy_map_set(&map, "name", name);

  char* value = hashy_map_get(&map, "name");

  HASHY_ASSERT(value != 0);
  HASHY_ASSERT(strcmp(value, pname) == 0);

  hashy_map_unset(&map, "name");

  value = hashy_map_get(&map, "name");

  HASHY_ASSERT(value == 0);

  value = hashy_map_set(&map, "name", name);

  HASHY_ASSERT(value != 0);
  HASHY_ASSERT(strcmp(value, pname) == 0);

  hashy_map_clear(&map, true);

  value = hashy_map_get(&map, "name");

  HASHY_ASSERT(value == 0);
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

void test_set_vectors() {
  int64_t n_vectors = 64;
  HashyMap map = {0};
  hashy_map_init(&map, 256);

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


      HASHY_ASSERT(hashy_map_set(&map, tmp, v) != 0);


      HashyBucket* bucket = hashy_map_get_bucket(&map, tmp);

      HASHY_ASSERT(bucket != 0);
      HASHY_ASSERT(bucket->key != 0);
      HASHY_ASSERT(strcmp(bucket->key, tmp) == 0);


      HashyVector* vv = hashy_map_get(&map, tmp);

      HASHY_ASSERT(vv  != 0);

      HASHY_ASSERT(vv->x == x);
      HASHY_ASSERT(vv->y == y);
      HASHY_ASSERT(vv->z == z);
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

      HASHY_ASSERT(bucket != 0);
      HASHY_ASSERT(bucket->key != 0);
      HASHY_ASSERT(strcmp(bucket->key, tmp) == 0);

      HashyVector* vv = hashy_map_get(&map, tmp);

      HASHY_ASSERT(vv->x == x);
      HASHY_ASSERT(vv->y == y);
      HASHY_ASSERT(vv->z == z);
    }
  }

  hashy_map_clear(&map, true);
}

void test_get_keys() {
  HashyMap map = {0};
  hashy_map_init(&map, 256);

  int64_t nr_items = 125;



  for (int64_t i = 0; i < nr_items; i++) {
    char tmp[256];
    sprintf(tmp, "item_%ld", i);
    char* x = strdup("123");
    hashy_map_set(&map, tmp, x);
  }



  HashyKeyList list = {0};
  hashy_map_get_keys(&map, &list);

  HASHY_ASSERT(list.length == nr_items);
  HASHY_ASSERT(list.items != 0);

  hashy_key_list_clear(&list);

  HASHY_ASSERT(list.items == 0);
  HASHY_ASSERT(list.length == 0);
  //HASHY_ASSERT(list.avail == 0);
  HASHY_ASSERT(list.length == 0);

  hashy_map_clear(&map, true);
}

void test_get_keys2() {
  HashyMap map = {0};
  hashy_map_init_v2(&map, (HashyMapConfig){ .capacity = 256, .remember_keys = false });

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

  HASHY_ASSERT(iter_count == nr_items);
  HASHY_ASSERT(it.keys.length <= 0);
  HASHY_ASSERT(it.keys.items == 0);


  char* s = hashy_map_unset(&map, "item_0");

  if (s) free(s);

  hashy_map_clear(&map, true);
  return;

  HashyKeyList* list = &map.keys;

  HASHY_ASSERT(list->length == nr_items);
  HASHY_ASSERT(list->items != 0);


  HASHY_ASSERT(list->length == nr_items-1);

  hashy_map_clear(&map, true);

  HASHY_ASSERT(list->items == 0);
  HASHY_ASSERT(list->length == 0);
  //HASHY_ASSERT(list->avail == 0);
  HASHY_ASSERT(list->length == 0);
}

void test_set_same() {
  HashyMap map = {0};
  hashy_map_init(&map, 256);

  const char* message = "hello world";
  char* mystr = strdup(message);

  int64_t iter = 100;
  const char* mykey = "message";

  for (int64_t i = 0; i < 100; i++) {
    hashy_map_set(&map, mykey, mystr);
  }

  for (int64_t i = 0; i < 100; i++) {
    char* value = hashy_map_get(&map, mykey);

    HASHY_ASSERT(value != 0);
    HASHY_ASSERT(strcmp(value, message) == 0);
  }

   for (int64_t i = 0; i < 100; i++) {
    hashy_map_set(&map, mykey, mystr);
  }

  for (int64_t i = 0; i < 100; i++) {
    char* value = hashy_map_get(&map, mykey);

    HASHY_ASSERT(value != 0);
    HASHY_ASSERT(strcmp(value, message) == 0);
  }



  hashy_map_clear(&map, true);
}

int main(int argc, char* argv[]) {

  srand(time(0));

  test_get_keys2();

  test_simple();
  test_big();
  test_get_without_set();
  test_unset_without_values();
  test_set_clear_and_get();
  test_set_vectors();
  test_get_keys();
  test_set_same();

  return 0;
}
