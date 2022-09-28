#include <assert.h>
#include <stdio.h>
#include "100vecs.h"

VEC_DECL(int);
typedef Vec_int* Vec_int_ptr;
VEC_DECL(Vec_int_ptr);

void test_nesting() {
  Vec_int* v1 = vec_create_int();
  vec_push_int(v1, 1);

  Vec_int* v2 = vec_create_int();
  vec_push_int(v2, 2);

  Vec_Vec_int_ptr* v = vec_create_Vec_int_ptr();
  vec_push_Vec_int_ptr(v, v1);
  vec_push_Vec_int_ptr(v, v2);

  assert(vec_size_Vec_int_ptr(v) == 2);
  assert(vec_get_int(vec_get_Vec_int_ptr(v, 0), 0) == 1);
  assert(vec_get_int(vec_get_Vec_int_ptr(v, 1), 0) == 2);

  vec_destroy_int(v1);
  vec_destroy_int(v2);
  vec_destroy_Vec_int_ptr(v);
}

VEC_IMPL(Vec_int_ptr)
