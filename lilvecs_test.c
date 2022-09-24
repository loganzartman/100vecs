#include <stdio.h>
#include "lilvecs.h"

void print_int(void* item, uint32_t i) {
  printf("index %d: %d\n", i, *((int*)item));
}

int main(int argc, char const *argv[]) {
  Vec v;
  vec_create(&v, sizeof(int));

  for (int i = 0; i < 100; ++i) {
    vec_push(&v, &i);
  }

  vec_foreach(&v, print_int);
  printf("vec capacity: %d\n", v.capacity);

  return 0;
}
