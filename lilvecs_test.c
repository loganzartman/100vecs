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

  return 0;
}

VEC_IMPL(int)
VEC_IMPL(Point)
