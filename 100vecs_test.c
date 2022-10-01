#include "100vecs.h"
#include <assert.h>
#include <stdio.h>

typedef struct Point {
  char* label;
  float x;
  float y;
} Point;

VEC_DECL(int)
VEC_DECL(Point)

void print_vec_int(Vec_int* v) {
  for (int i = 0; i < vec_size_int(v); ++i) {
    if (i > 0) {
      printf(", ");
    }
    printf("%d", vec_get_int(v, i));
  }
  printf("\n");
}

void test_create() {
  Vec_int* vi = vec_create_int();
  assert(vi);
  assert(&*(char*)vi);
  vec_destroy_int(vi);

  Vec_Point* vp = vec_create_Point();
  assert(vp);
  assert(&*(char*)vp);
  vec_destroy_Point(vp);
}

void test_destroy() {
  Vec_int* vi = vec_create_int();
  vec_destroy_int(vi);

  Vec_Point* vp = vec_create_Point();
  vec_destroy_Point(vp);
}

void test_clone() {
  Vec_int* v = vec_create_int();
  vec_push_int(v, 1);
  vec_push_int(v, 2);
  vec_push_int(v, 3);

  Vec_int* clone = vec_clone_int(v);
  assert(vec_size_int(clone) == 3);
  assert(vec_get_int(clone, 0) == 1);
  assert(vec_get_int(clone, 1) == 2);
  assert(vec_get_int(clone, 2) == 3);

  vec_destroy_int(v);
  vec_destroy_int(clone);
}

void test_grow() {
  Vec_int* vi = vec_create_int();
  assert(vec_capacity_int(vi) == 0);
  vec_grow_int(vi, 10);
  assert(vec_capacity_int(vi) >= 10);
  vec_destroy_int(vi);

  Vec_Point* vp = vec_create_Point();
  assert(vec_capacity_Point(vp) == 0);
  vec_grow_Point(vp, 10);
  assert(vec_capacity_Point(vp) >= 10);
  vec_destroy_Point(vp);
}

void test_push() {
  Vec_int* vi = vec_create_int();
  assert(vec_size_int(vi) == 0);
  vec_push_int(vi, 10);
  assert(vec_size_int(vi) == 1);
  vec_push_int(vi, 20);
  assert(vec_size_int(vi) == 2);
  assert(vec_get_int(vi, 1) == 20);
  vec_destroy_int(vi);

  Vec_Point* vp = vec_create_Point();
  assert(vec_size_Point(vp) == 0);
  vec_push_Point(vp, (Point){.label = "spot", .x = 420.0, .y = 69.0});
  assert(vec_size_Point(vp) == 1);
  vec_push_Point(vp, (Point){.label = "point", .x = 1337.0, .y = 42.0});
  assert(vec_size_Point(vp) == 2);
  assert(vec_get_Point(vp, 1).x == 1337.0);
  vec_destroy_Point(vp);
}

void test_pop() {
  Vec_int* vi = vec_create_int();
  vec_push_int(vi, 10);
  vec_push_int(vi, 20);
  vec_push_int(vi, 30);
  assert(vec_size_int(vi) == 3);
  assert(vec_pop_int(vi) == 30);
  assert(vec_size_int(vi) == 2);
  assert(vec_pop_int(vi) == 20);
  assert(vec_size_int(vi) == 1);
  assert(vec_pop_int(vi) == 10);
  assert(vec_size_int(vi) == 0);
  vec_destroy_int(vi);
}

void test_compact() {
  Vec_int* vi = vec_create_int();
  for (int i = 0; i < 10; ++i) {
    vec_push_int(vi, i);
  }
  vec_pop_int(vi);
  assert(vec_capacity_int(vi) >= 10);
  vec_compact_int(vi);
  assert(vec_capacity_int(vi) == 9);
  vec_compact_int(vi);
  assert(vec_capacity_int(vi) == 9);

  vec_destroy_int(vi);
}

void test_set() {
  Vec_int* vi = vec_create_int();
  vec_push_int(vi, 10);
  vec_push_int(vi, 20);
  vec_push_int(vi, 30);

  vec_set_int(vi, 1, 40);
  assert(vec_get_int(vi, 0) == 10);
  assert(vec_get_int(vi, 1) == 40);
  assert(vec_get_int(vi, 2) == 30);
  vec_destroy_int(vi);
}

void test_cleanup() {
  {
    Vec_int* v VEC_CLEANUP(int) = vec_create_int();
    vec_push_int(v, 10);
  }
}

void test_insert() {
  // middle
  {
    Vec_int* v = vec_create_int();
    vec_push_int(v, 10);
    vec_push_int(v, 20);
    vec_push_int(v, 30);

    vec_insert_int(v, 1, 15);
    assert(vec_size_int(v) == 4);
    assert(vec_get_int(v, 0) == 10);
    assert(vec_get_int(v, 1) == 15);
    assert(vec_get_int(v, 2) == 20);
    assert(vec_get_int(v, 3) == 30);
    vec_destroy_int(v);
  }
  // beginning
  {
    Vec_int* v = vec_create_int();
    vec_push_int(v, 10);
    vec_push_int(v, 20);
    vec_push_int(v, 30);

    vec_insert_int(v, 0, 5);
    assert(vec_size_int(v) == 4);
    assert(vec_get_int(v, 0) == 5);
    assert(vec_get_int(v, 1) == 10);
    assert(vec_get_int(v, 2) == 20);
    assert(vec_get_int(v, 3) == 30);
    vec_destroy_int(v);
  }
  // end
  {
    Vec_int* v = vec_create_int();
    vec_push_int(v, 10);
    vec_push_int(v, 20);
    vec_push_int(v, 30);

    vec_insert_int(v, 3, 35);
    assert(vec_size_int(v) == 4);
    assert(vec_get_int(v, 0) == 10);
    assert(vec_get_int(v, 1) == 20);
    assert(vec_get_int(v, 2) == 30);
    assert(vec_get_int(v, 3) == 35);
    vec_destroy_int(v);
  }
  // empty
  {
    Vec_int* v = vec_create_int();

    vec_insert_int(v, 0, 10);
    assert(vec_size_int(v) == 1);
    assert(vec_get_int(v, 0) == 10);
    vec_destroy_int(v);
  }
}

void test_remove() {
  // middle
  {
    Vec_int* v = vec_create_int();
    vec_push_int(v, 10);
    vec_push_int(v, 20);
    vec_push_int(v, 30);

    int removed = vec_remove_int(v, 1);
    assert(removed == 20);
    assert(vec_size_int(v) == 2);
    assert(vec_get_int(v, 0) == 10);
    assert(vec_get_int(v, 1) == 30);
    vec_destroy_int(v);
  }
  // beginning
  {
    Vec_int* v = vec_create_int();
    vec_push_int(v, 10);
    vec_push_int(v, 20);
    vec_push_int(v, 30);

    int removed = vec_remove_int(v, 0);
    assert(removed == 10);
    assert(vec_size_int(v) == 2);
    assert(vec_get_int(v, 0) == 20);
    assert(vec_get_int(v, 1) == 30);
    vec_destroy_int(v);
  }
  // end
  {
    Vec_int* v = vec_create_int();
    vec_push_int(v, 10);
    vec_push_int(v, 20);
    vec_push_int(v, 30);

    int removed = vec_remove_int(v, 2);
    assert(removed == 30);
    assert(vec_size_int(v) == 2);
    assert(vec_get_int(v, 0) == 10);
    assert(vec_get_int(v, 1) == 20);
    vec_destroy_int(v);
  }
  // only item
  {
    Vec_int* v = vec_create_int();
    vec_push_int(v, 10);

    int removed = vec_remove_int(v, 0);
    assert(removed == 10);
    assert(vec_size_int(v) == 0);
    vec_destroy_int(v);
  }
}

void test_insert_items() {
  // middle
  {
    Vec_int* v = vec_create_int();
    vec_push_int(v, 10);
    vec_push_int(v, 20);
    vec_push_int(v, 30);

    Vec_int* v2 = vec_create_int();
    vec_push_int(v2, 11);
    vec_push_int(v2, 12);

    vec_insert_items_int(v, 1, vec_size_int(v2), vec_data_int(v2));
    assert(vec_size_int(v) == 5);
    assert(vec_get_int(v, 0) == 10);
    assert(vec_get_int(v, 1) == 11);
    assert(vec_get_int(v, 2) == 12);
    assert(vec_get_int(v, 3) == 20);
    assert(vec_get_int(v, 4) == 30);
    vec_destroy_int(v);
    vec_destroy_int(v2);
  }
}

int int_comparator(const int* a, const int* b) {
  return (*a > *b) - (*a < *b);
}

void test_qsort() {
  Vec_int* v = vec_create_int();
  vec_push_int(v, 10);
  vec_push_int(v, 30);
  vec_push_int(v, 20);

  vec_qsort_int(v, int_comparator);
  assert(vec_get_int(v, 0) == 10);
  assert(vec_get_int(v, 1) == 20);
  assert(vec_get_int(v, 2) == 30);

  vec_destroy_int(v);
}

extern void test_nesting();

extern void test_map();

int main(int argc, char const* argv[]) {
  printf("test_create\n");
  test_create();
  printf("test_destroy\n");
  test_destroy();
  printf("test_clone\n");
  test_clone();
  printf("test_grow\n");
  test_grow();
  printf("test_push\n");
  test_push();
  printf("test_pop\n");
  test_pop();
  printf("test_compact\n");
  test_compact();
  printf("test_set\n");
  test_set();
  printf("test_cleanup\n");
  test_cleanup();
  printf("test_insert\n");
  test_insert();
  printf("test_remove\n");
  test_remove();
  printf("test_insert_items\n");
  test_insert_items();
  printf("test_qsort\n");
  test_qsort();

  printf("test_nesting\n");
  test_nesting();

  printf("test_map\n");
  test_map();

  return 0;
}

VEC_IMPL(int)
VEC_IMPL(Point)
