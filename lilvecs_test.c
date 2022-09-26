#include "lilvecs.h"
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

  Vec_Point* vp = vec_create_Point();
  assert(vp);
  assert(&*(char*)vp);
}

void test_delete() {
  Vec_int* vi = vec_create_int();
  vec_delete_int(vi);

  Vec_Point* vp = vec_create_Point();
  vec_delete_Point(vp);
}

void test_grow() {
  Vec_int* vi = vec_create_int();
  assert(vec_capacity_int(vi) == 0);
  vec_grow_int(vi, 10);
  assert(vec_capacity_int(vi) >= 10);
  vec_delete_int(vi);

  Vec_Point* vp = vec_create_Point();
  assert(vec_capacity_Point(vp) == 0);
  vec_grow_Point(vp, 10);
  assert(vec_capacity_Point(vp) >= 10);
  vec_delete_Point(vp);
}

void test_push() {
  Vec_int* vi = vec_create_int();
  assert(vec_size_int(vi) == 0);
  vec_push_int(vi, 10);
  assert(vec_size_int(vi) == 1);
  vec_push_int(vi, 20);
  assert(vec_size_int(vi) == 2);
  assert(vec_get_int(vi, 1) == 20);
  vec_delete_int(vi);

  Vec_Point* vp = vec_create_Point();
  assert(vec_size_Point(vp) == 0);
  vec_push_Point(vp, (Point){
    .label="spot",
    .x=420.0,
    .y=69.0
  });
  assert(vec_size_Point(vp) == 1);
  vec_push_Point(vp, (Point){
    .label="point",
    .x=1337.0,
    .y=42.0
  });
  assert(vec_size_Point(vp) == 2);
  assert(vec_get_Point(vp, 1).x == 1337.0);
  vec_delete_Point(vp);
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
  vec_delete_int(vi);
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
  vec_delete_int(vi);
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
    vec_delete_int(v);
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
    vec_delete_int(v);
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
    vec_delete_int(v);
  }
  // empty
  {
    Vec_int* v = vec_create_int();

    vec_insert_int(v, 0, 10);
    assert(vec_size_int(v) == 1);
    assert(vec_get_int(v, 0) == 10);
    vec_delete_int(v);
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
    vec_delete_int(v);
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
    vec_delete_int(v);
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
    vec_delete_int(v);
  }
  // only item
  {
    Vec_int* v = vec_create_int();
    vec_push_int(v, 10);

    int removed = vec_remove_int(v, 0);
    assert(removed == 10);
    assert(vec_size_int(v) == 0);
    vec_delete_int(v);
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
    vec_delete_int(v);
    vec_delete_int(v2);
  }
}

int main(int argc, char const* argv[]) {
  printf("test_create\n");
  test_create();
  printf("test_delete\n");
  test_delete();
  printf("test_grow\n");
  test_grow();
  printf("test_push\n");
  test_push();
  printf("test_pop\n");
  test_pop();
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

  return 0;
}

VEC_IMPL(int)
VEC_IMPL(Point)
