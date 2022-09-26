#include "lilvecs.h"
#include <stdio.h>

VEC_DECL(int)

void print_int(int item, uint32_t i) {
  printf("index %d: %d\n", i, item);
}

int main(int argc, char const* argv[]) {
  Vec_int* v = vec_create_int();

  for (int i = 0; i < 100; ++i) {
    vec_push_int(v, i);
  }

  vec_foreach_int(v, print_int);
  printf("vec capacity: %d\n", vec_capacity_int(v));

  vec_delete_int(v);
  return 0;
}

VEC_IMPL(int)
