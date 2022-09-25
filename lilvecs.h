#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(VEC_GROW_FACTOR)
#define VEC_GROW_FACTOR 1.5
#endif

#define VEC_INSTANCE(T)                                                      \
  typedef struct Vec_##T {                                                   \
    uint32_t size;                                                           \
    uint32_t capacity;                                                       \
    uint8_t* data;                                                           \
  } Vec_##T;                                                                 \
                                                                             \
  void vec_create_##T(Vec_##T* v) {                                          \
    assert(v);                                                               \
                                                                             \
    v->size = 0;                                                             \
    v->capacity = 0;                                                         \
    v->data = NULL;                                                          \
  }                                                                          \
                                                                             \
  void vec_clear_##T(Vec_##T* v) {                                           \
    assert(v);                                                               \
                                                                             \
    if (v->data) {                                                           \
      v->size = 0;                                                           \
      v->capacity = 0;                                                       \
      free(v->data);                                                         \
      v->data = NULL;                                                        \
    }                                                                        \
  }                                                                          \
                                                                             \
  void vec_grow_##T(Vec_##T* v, uint32_t capacity) {                         \
    assert(v);                                                               \
                                                                             \
    if (capacity > v->capacity) {                                            \
      uint32_t new_capacity = (uint32_t)(v->capacity * VEC_GROW_FACTOR) + 1; \
      if (v->data) {                                                         \
        v->data = realloc(v->data, new_capacity * sizeof(T));                \
      } else {                                                               \
        v->data = malloc(new_capacity * sizeof(T));                          \
      }                                                                      \
      v->capacity = new_capacity;                                            \
    }                                                                        \
  }                                                                          \
                                                                             \
  T* vec_get_##T(Vec_##T* v, uint32_t i) {                                   \
    assert(v);                                                               \
    assert(i >= 0);                                                          \
    assert(i < v->size);                                                     \
                                                                             \
    return (T*)(v->data + i * sizeof(T));                                    \
  }                                                                          \
                                                                             \
  void vec_set_##T(Vec_##T* v, uint32_t i, T* item) {                        \
    assert(v);                                                               \
    assert(i >= 0);                                                          \
    assert(i < v->size);                                                     \
                                                                             \
    memcpy(v->data + i * sizeof(T), item, sizeof(T));                        \
  }                                                                          \
                                                                             \
  void vec_push_##T(Vec_##T* v, T* item) {                                   \
    assert(v);                                                               \
                                                                             \
    vec_grow_##T(v, v->size + 1);                                            \
    v->size += 1;                                                            \
    vec_set_##T(v, v->size - 1, item);                                       \
  }                                                                          \
                                                                             \
  T* vec_pop_##T(Vec_##T* v) {                                               \
    assert(v);                                                               \
    assert(v->size > 0);                                                     \
                                                                             \
    v->size -= 1;                                                            \
    return vec_get_##T(v, v->size);                                          \
  }                                                                          \
                                                                             \
  void vec_foreach_##T(Vec_##T* v, void fn(T*, uint32_t)) {                  \
    for (int i = 0; i < v->size; ++i) {                                      \
      fn(vec_get_##T(v, i), i);                                              \
    }                                                                        \
  }
