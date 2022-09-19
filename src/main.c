#include <hashy/hashy.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>


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

    char* back_value = hashy_map_get(&map, key);

    HASHY_ASSERT(back_value != 0);
    HASHY_ASSERT(strcmp(back_value, expected) == 0);
  }

}

int main(int argc, char* argv[]) {

  test_simple();
  test_big();

  return 0;
}
