# lilvecs

_super naïve type-templated vector functions in C_

## Testimonials

> "An objectively bad idea. 😒"

–some individual whomst write C for a living

## Overview

* Header only! `#include "lilvecs.h"`
* Run tests: `make test`
* [List of functions](function_list.md)

## Basic usage

```C
#include "lilvecs.h"
#include <stdio.h>

// generate definitions for vectors of ints
VEC_IMPL(int)

int main(int argc, char const* argv[]) {
  // use real structs and functions!
  Vec_int* v = vec_create_int();

  for (int i = 0; i < 100; ++i) {
    vec_push_int(v, i);
  }

  // no void* or other tomfoolery!
  int item = vec_get_int(v, 42);
  printf("v[42] = %d\n", item);
  printf("size: %d, capacity: %d\n", vec_size_int(v), vec_capacity_int(v));

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

// create a vector of literally any type!
// (as long as it's a valid identifier)
// (otherwise you need to typedef it first)
VEC_IMPL(Point)

int main(int argc, char const* argv[]) {
  // automatic cleanup with VEC_CLEANUP macro!
  Vec_Point* v VEC_CLEANUP(Point) = vec_create_Point();

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
  return 0;
}
```

## Forward declarations

`VEC_IMPL` generates implementations, which can only appear once per compilation unit (`.c` file).

`VEC_DECL` generates forward declarations, which can be repeated as many times as you want! You can use this to access vector functions in header files.
The only limitation is that the Vec_T structs are incomplete and can only be referenced as pointers, but I've designed the API around that anyway. You still need a `VEC_IMPL` for each type somewhere!

```C
#include "lilvecs.h"
#include <stdio.h>

// these can go wherever you want to call vector functions!
VEC_DECL(float)

int main(int argc, char const* argv[]) {
  Vec_float* v VEC_CLEANUP(float) = vec_create_float();
  vec_push_float(v, 1.337);
  printf("%f\n", vec_get_float(v, 0));
  return 0;
}

// need exactly one of these per compilation unit
VEC_IMPL(float)
```