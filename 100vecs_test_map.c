#include <assert.h>
#include <stdio.h>
#include "100vecs.h"

MAP_DECL(int, int)

int hash_int(int i) {
  return i;
}

bool eq_int(int a, int b) {
  return a == b;
}

void test_map_put() {
  Map_int__int* m = map_create_int__int(hash_int, eq_int);
  map_put_int__int(m, 1, 10);
  map_put_int__int(m, 5, 420);

  map_destroy_int__int(m);
}

void test_map_grow() {
  Map_int__int* m = map_create_int__int(hash_int, eq_int);
  map_put_int__int(m, 1, 10);
  printf("  size: %d, capacity: %d\n", map_size_int__int(m),
         map_capacity_int__int(m));
  assert(map_capacity_int__int(m) > 1 / MAP_MAX_LOAD);

  for (int i = 1; i < 10; ++i) {
    map_put_int__int(m, i, i * 10);
  }
  printf("  size: %d, capacity: %d\n", map_size_int__int(m),
         map_capacity_int__int(m));
  assert(map_capacity_int__int(m) > 10 / MAP_MAX_LOAD);

  for (int i = 10; i < 100; ++i) {
    map_put_int__int(m, i, i * 10);
  }
  printf("  size: %d, capacity: %d\n", map_size_int__int(m),
         map_capacity_int__int(m));
  assert(map_capacity_int__int(m) > 100 / MAP_MAX_LOAD);

  map_destroy_int__int(m);
}

void test_map_shrink() {
  Map_int__int* m = map_create_int__int(hash_int, eq_int);
  for (int i = 0; i < 100; ++i) {
    map_put_int__int(m, i, i * 10);
  }

  printf("  size: %d, capacity: %d\n", map_size_int__int(m),
         map_capacity_int__int(m));
  assert(map_capacity_int__int(m) > 100 / MAP_MAX_LOAD);

  for (int i = 0; i < 75; ++i) {
    map_delete_int__int(m, i);
  }

  printf("  size: %d, capacity: %d\n", map_size_int__int(m),
         map_capacity_int__int(m));
  assert(map_capacity_int__int(m) > 25 / MAP_MAX_LOAD);
  assert(map_capacity_int__int(m) < 100 / MAP_MAX_LOAD);

  for (int i = 75; i < 90; ++i) {
    map_delete_int__int(m, i);
  }

  printf("  size: %d, capacity: %d\n", map_size_int__int(m),
         map_capacity_int__int(m));
  assert(map_capacity_int__int(m) > 10 / MAP_MAX_LOAD);
  assert(map_capacity_int__int(m) < 25 / MAP_MAX_LOAD);

  map_destroy_int__int(m);
}

void test_map_get() {
  Map_int__int* m = map_create_int__int(hash_int, eq_int);
  map_put_int__int(m, 1, 10);
  map_put_int__int(m, 5, 420);

  assert(map_get_int__int(m, 1) != NULL);
  assert(*map_get_int__int(m, 1) == 10);

  assert(map_get_int__int(m, 5) != NULL);
  assert(*map_get_int__int(m, 5) == 420);

  assert(map_get_int__int(m, 2) == NULL);

  map_destroy_int__int(m);
}

void test_map_has() {
  Map_int__int* m = map_create_int__int(hash_int, eq_int);
  map_put_int__int(m, 1, 10);

  assert(map_has_int__int(m, 1));
  assert(!map_has_int__int(m, 2));

  map_destroy_int__int(m);
}

void test_map_get_else() {
  Map_int__int* m = map_create_int__int(hash_int, eq_int);
  map_put_int__int(m, 1, 10);

  assert(map_get_else_int__int(m, 1, -1) == 10);
  assert(map_get_else_int__int(m, 2, -1) == -1);

  map_destroy_int__int(m);
}

void test_map_delete() {
  Map_int__int* m = map_create_int__int(hash_int, eq_int);
  map_put_int__int(m, 1, 10);
  map_put_int__int(m, 2, 20);
  map_put_int__int(m, 3, 30);

  assert(map_size_int__int(m) == 3);

  assert(map_has_int__int(m, 1));
  assert(map_delete_int__int(m, 1));
  assert(!map_has_int__int(m, 1));
  assert(map_size_int__int(m) == 2);

  assert(map_has_int__int(m, 3));
  assert(map_delete_int__int(m, 3));
  assert(!map_has_int__int(m, 3));
  assert(map_size_int__int(m) == 1);

  assert(map_has_int__int(m, 2));
  assert(map_delete_int__int(m, 2));
  assert(!map_has_int__int(m, 2));
  assert(map_size_int__int(m) == 0);

  assert(!map_delete_int__int(m, 1));

  map_destroy_int__int(m);
}

int dummy_hash(int i) {
  if (i < 5)
    return 0;
  return 1;
}

void test_map_collisions() {
  Map_int__int* m = map_create_int__int(dummy_hash, eq_int);

  map_put_int__int(m, 1, 10);
  assert(map_has_int__int(m, 1));
  assert(*map_get_int__int(m, 1) == 10);

  map_put_int__int(m, 2, 20);
  assert(map_has_int__int(m, 1));
  assert(*map_get_int__int(m, 1) == 10);
  assert(map_has_int__int(m, 2));
  assert(*map_get_int__int(m, 2) == 20);

  map_put_int__int(m, 3, 30);
  assert(map_has_int__int(m, 1));
  assert(*map_get_int__int(m, 1) == 10);
  assert(map_has_int__int(m, 2));
  assert(*map_get_int__int(m, 2) == 20);
  assert(map_has_int__int(m, 3));
  assert(*map_get_int__int(m, 3) == 30);

  map_delete_int__int(m, 2);
  assert(map_has_int__int(m, 1));
  assert(*map_get_int__int(m, 1) == 10);
  assert(map_has_int__int(m, 3));
  assert(*map_get_int__int(m, 3) == 30);

  map_destroy_int__int(m);
  m = map_create_int__int(dummy_hash, eq_int);

  map_put_int__int(m, 1, 10);
  map_put_int__int(m, 2, 20);
  map_put_int__int(m, 5, 50);
  map_put_int__int(m, 6, 60);
  assert(map_has_int__int(m, 1));
  assert(*map_get_int__int(m, 1) == 10);
  assert(map_has_int__int(m, 2));
  assert(*map_get_int__int(m, 2) == 20);
  assert(map_has_int__int(m, 5));
  assert(*map_get_int__int(m, 5) == 50);
  assert(map_has_int__int(m, 6));
  assert(*map_get_int__int(m, 6) == 60);

  map_destroy_int__int(m);
}

void test_map() {
  printf("test_map_put\n");
  test_map_put();
  printf("test_map_grow\n");
  test_map_grow();
  printf("test_map_shrink\n");
  test_map_shrink();
  printf("test_map_get\n");
  test_map_get();
  printf("test_map_get_else\n");
  test_map_get_else();
  printf("test_map_has\n");
  test_map_has();
  printf("test_map_delete\n");
  test_map_delete();
  printf("test_map_collisions\n");
  test_map_collisions();
}

MAP_IMPL(int, int)
