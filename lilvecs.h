#pragma once
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#if !defined(VEC_GROW_FACTOR)
#define VEC_GROW_FACTOR 1.5
#endif

typedef struct Vec {
  uint32_t size;
  uint32_t capacity;
  uint8_t element_size;
  uint8_t* data;
} Vec;

void vec_create(Vec* v, uint8_t element_size) {
  assert(v);

  v->size = 0;
  v->capacity = 0;
  v->element_size = element_size;
  v->data = NULL;
}

void vec_create_len(Vec* v, uint8_t element_size, uint32_t capacity) {
  assert(v);

  v->size = 0;
  v->capacity = capacity;
  v->element_size = element_size;
  v->data = calloc(capacity, element_size);
}

void vec_clear(Vec* v) {
  assert(v);

  if (v->data) {
    v->size = 0;
    v->capacity = 0;
    free(v->data);
    v->data = NULL;
  }
}

void vec_grow(Vec* v, uint32_t capacity) {
  assert(v);
  
  if (capacity > v->capacity) {
    uint32_t new_capacity = (uint32_t)(v->capacity * VEC_GROW_FACTOR) + 1;
    uint8_t* new_data = calloc(new_capacity, v->element_size);
    if (v->data) {
      memcpy(new_data, v->data, v->size * v->element_size);
      free(v->data);
    }
    v->capacity = new_capacity;
    v->data = new_data;
  }
}

void* vec_get(Vec* v, uint32_t i) {
  assert(v);
  assert(i >= 0);
  assert(i < v->size);

  return v->data + i * v->element_size;
}

void vec_set(Vec* v, uint32_t i, void* item) {
  assert(v);
  assert(i >= 0);
  assert(i < v->size);

  memcpy(v->data + i * v->element_size, item, v->element_size);
}

void vec_push(Vec* v, void* item) {
  assert(v);

  vec_grow(v, v->size + 1);
  v->size += 1;
  vec_set(v, v->size - 1, item);
}

void* vec_pop(Vec* v) {
  assert(v);
  assert(v->size > 0);

  v->size -= 1;
  return vec_get(v, v->size);
}

void* vec_foreach(Vec* v, void fn(void*, uint32_t)) {
  for (int i = 0; i < v->size; ++i) {
    fn(vec_get(v, i), i);
  }
}
