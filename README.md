# lilvecs

_super naïve type-templated vector functions in C_

## Testimonials

> "An objectively bad idea. 😒"

–some individual whomst write C for a living

## Building

* Header only! `#include "lilvecs.h"`
* Run tests: `make test`

## Basic usage

```C
#include "lilvecs.h"
#include <stdio.h>

VEC_IMPL(int)

int main(int argc, char const* argv[]) {
  Vec_int* v = vec_create_int();

  for (int i = 0; i < 100; ++i) {
    vec_push_int(v, i);
  }

  printf("v[42] = %d\n", vec_get_int(v, 42));
  printf("size: %d, capacity: %d\n", vec_size(v), vec_capacity(v));

  vec_delete_int(v);
  return 0;
}
```

## Store anything

```C
#include "../lilvecs.h"
#include <stdio.h>

typedef struct Point {
  float x;
  float y;
  char* label;
} Point;

VEC_IMPL(Point)

int main(int argc, char const* argv[]) {
  Vec_Point* v = vec_create_Point();

  vec_push_Point(v, (Point){
    .x=3.14,
    .y=6.28,
    .label="place"
  });
  vec_push_Point(v, (Point){
    .x=42.0,
    .y=420.0,
    .label="spot"
  });

  Point p = vec_get_Point(v, 1);
  printf("v[1] = %s (%f, %f)\n", p.label, p.x, p.y);

  vec_delete_Point(v);
  return 0;
}
```

## Forward declarations

For using vectors in header files; the same VEC_DECL may appear several times.

```C
#include "lilvecs.h"
#include <stdio.h>

// put these wherever to start calling library functions
VEC_DECL(float)

int main(int argc, char const* argv[]) {
  Vec_float* v = vec_create_float();
  vec_push_float(v, 1.337);
  vec_delete_float(v);

  return 0;
}

// need exactly one of these per compilation unit
VEC_IMPL(float)
```