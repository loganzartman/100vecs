#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(VEC_GROW_FACTOR)
#define VEC_GROW_FACTOR 1.5
#endif

#if !defined(VEC_DATA_POINTERS)
#define VEC_DATA_POINTERS 0
#endif

#if VEC_DATA_POINTERS
#define ITEM_TYPE(T) T*
#define ITEM_DEREF(item) (item)
#define ITEM_REF(item) (item)
#else
#define ITEM_TYPE(T) T
#define ITEM_DEREF(item) *(item)
#define ITEM_REF(item) &(item)
#endif

#define VEC_DECL(T)                                                   \
  typedef struct Vec_##T Vec_##T;                                     \
  extern Vec_##T* vec_create_##T();                                   \
  extern void vec_delete_##T(Vec_##T* v);                             \
  extern uint32_t vec_capacity_##T(Vec_##T* v);                       \
  extern uint32_t vec_size_##T(Vec_##T* v);                           \
  extern void vec_clear_##T(Vec_##T* v);                              \
  extern void vec_grow_##T(Vec_##T* v, uint32_t capacity);            \
  extern ITEM_TYPE(T) vec_get_##T(Vec_##T* v, uint32_t i);            \
  extern void vec_set_##T(Vec_##T* v, uint32_t i, ITEM_TYPE(T) item); \
  extern void vec_push_##T(Vec_##T* v, ITEM_TYPE(T) item);            \
  extern ITEM_TYPE(T) vec_pop_##T(Vec_##T* v);                        \
  extern void vec_foreach_##T(Vec_##T* v, void fn(ITEM_TYPE(T), uint32_t));

#define VEC_IMPL(T)                                                          \
  typedef struct Vec_##T {                                                   \
    uint32_t size;                                                           \
    uint32_t capacity;                                                       \
    uint8_t* data;                                                           \
  } Vec_##T;                                                                 \
                                                                             \
  Vec_##T* vec_create_##T() {                                                \
    Vec_##T* v = malloc(sizeof(Vec_##T));                                    \
    v->size = 0;                                                             \
    v->capacity = 0;                                                         \
    v->data = NULL;                                                          \
    return v;                                                                \
  }                                                                          \
                                                                             \
  void vec_delete_##T(Vec_##T* v) {                                          \
    if (v->data) {                                                           \
      free(v->data);                                                         \
    }                                                                        \
    free(v);                                                                 \
  }                                                                          \
                                                                             \
  uint32_t vec_capacity_##T(Vec_##T* v) {                                    \
    return v->capacity;                                                      \
  }                                                                          \
                                                                             \
  uint32_t vec_size_##T(Vec_##T* v) {                                        \
    return v->size;                                                          \
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
  ITEM_TYPE(T) vec_get_##T(Vec_##T* v, uint32_t i) {                         \
    assert(v);                                                               \
    assert(i >= 0);                                                          \
    assert(i < v->size);                                                     \
                                                                             \
    return ITEM_DEREF((T*)(v->data + i * sizeof(T)));                        \
  }                                                                          \
                                                                             \
  void vec_set_##T(Vec_##T* v, uint32_t i, ITEM_TYPE(T) item) {              \
    assert(v);                                                               \
    assert(i >= 0);                                                          \
    assert(i < v->size);                                                     \
                                                                             \
    memcpy(v->data + i * sizeof(T), ITEM_REF(item), sizeof(T));              \
  }                                                                          \
                                                                             \
  void vec_push_##T(Vec_##T* v, ITEM_TYPE(T) item) {                         \
    assert(v);                                                               \
                                                                             \
    vec_grow_##T(v, v->size + 1);                                            \
    v->size += 1;                                                            \
    vec_set_##T(v, v->size - 1, item);                                       \
  }                                                                          \
                                                                             \
  ITEM_TYPE(T) vec_pop_##T(Vec_##T* v) {                                     \
    assert(v);                                                               \
    assert(v->size > 0);                                                     \
                                                                             \
    v->size -= 1;                                                            \
    return vec_get_##T(v, v->size);                                          \
  }                                                                          \
                                                                             \
  void vec_foreach_##T(Vec_##T* v, void fn(ITEM_TYPE(T), uint32_t)) {        \
    for (int i = 0; i < v->size; ++i) {                                      \
      fn(vec_get_##T(v, i), i);                                              \
    }                                                                        \
  }
