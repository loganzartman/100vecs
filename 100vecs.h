#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(VEC_SIZE_T)
#define VEC_SIZE_T uint32_t
#endif

#if !defined(VEC_GROW_FACTOR)
#define VEC_GROW_FACTOR 1.5
#endif

#if !defined(VEC_DATA_POINTERS)
#define VEC_DATA_POINTERS 0
#endif

#if VEC_DATA_POINTERS
#define ITEM_TYPE(T) T*
#define ITEM_DEREF(item) (item)
#define ITEM_REF(item) *(item)
#else
#define ITEM_TYPE(T) T
#define ITEM_DEREF(item) *(item)
#define ITEM_REF(item) (item)
#endif

#define VEC_CLEANUP(T) __attribute__((__cleanup__(vec_cleanup_##T)))

#define VEC_DECL(T)                                                            \
  typedef struct Vec_##T Vec_##T;                                              \
  extern Vec_##T* vec_create_##T();                                            \
  extern void vec_delete_##T(Vec_##T* v);                                      \
  extern void vec_cleanup_##T(Vec_##T** v);                                    \
  extern VEC_SIZE_T vec_size_##T(Vec_##T* v);                                  \
  extern VEC_SIZE_T vec_capacity_##T(Vec_##T* v);                              \
  extern T* vec_data_##T(Vec_##T* v);                                          \
  extern void vec_clear_##T(Vec_##T* v);                                       \
  extern void vec_grow_##T(Vec_##T* v, VEC_SIZE_T capacity);                   \
  extern void vec_compact_##T(Vec_##T* v);                                     \
  extern ITEM_TYPE(T) vec_get_##T(Vec_##T* v, VEC_SIZE_T index);               \
  extern void vec_set_##T(Vec_##T* v, VEC_SIZE_T index, ITEM_TYPE(T) item);    \
  extern void vec_push_##T(Vec_##T* v, ITEM_TYPE(T) item);                     \
  extern ITEM_TYPE(T) vec_pop_##T(Vec_##T* v);                                 \
  extern void vec_insert_##T(Vec_##T* v, VEC_SIZE_T index, ITEM_TYPE(T) item); \
  extern void vec_insert_items_##T(Vec_##T* v, VEC_SIZE_T index,               \
                                   VEC_SIZE_T count, T* items);                \
  extern ITEM_TYPE(T) vec_remove_##T(Vec_##T* v, VEC_SIZE_T index);            \
  extern void vec_foreach_##T(Vec_##T* v, void fn(ITEM_TYPE(T), VEC_SIZE_T));

#define VEC_IMPL(T)                                                            \
  typedef struct Vec_##T {                                                     \
    VEC_SIZE_T size;                                                           \
    VEC_SIZE_T capacity;                                                       \
    T* data;                                                                   \
  } Vec_##T;                                                                   \
                                                                               \
  Vec_##T* vec_create_##T() {                                                  \
    Vec_##T* v = malloc(sizeof(Vec_##T));                                      \
    v->size = 0;                                                               \
    v->capacity = 0;                                                           \
    v->data = NULL;                                                            \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  void vec_delete_##T(Vec_##T* v) {                                            \
    if (v->data) {                                                             \
      free(v->data);                                                           \
      v->data = NULL;                                                          \
    }                                                                          \
    free(v);                                                                   \
  }                                                                            \
                                                                               \
  void vec_cleanup_##T(Vec_##T** v) {                                          \
    vec_delete_##T(*v);                                                        \
  }                                                                            \
                                                                               \
  VEC_SIZE_T vec_size_##T(Vec_##T* v) {                                        \
    return v->size;                                                            \
  }                                                                            \
                                                                               \
  VEC_SIZE_T vec_capacity_##T(Vec_##T* v) {                                    \
    return v->capacity;                                                        \
  }                                                                            \
                                                                               \
  T* vec_data_##T(Vec_##T* v) {                                                \
    return v->data;                                                            \
  }                                                                            \
                                                                               \
  void vec_clear_##T(Vec_##T* v) {                                             \
    assert(v);                                                                 \
                                                                               \
    if (v->data) {                                                             \
      v->size = 0;                                                             \
      v->capacity = 0;                                                         \
      free(v->data);                                                           \
      v->data = NULL;                                                          \
    }                                                                          \
  }                                                                            \
                                                                               \
  void vec_grow_##T(Vec_##T* v, VEC_SIZE_T capacity) {                         \
    assert(v);                                                                 \
                                                                               \
    if (capacity > v->capacity) {                                              \
      VEC_SIZE_T capacity_diff = capacity - v->capacity;                       \
      VEC_SIZE_T new_capacity =                                                \
          (VEC_SIZE_T)(v->capacity * VEC_GROW_FACTOR) + capacity_diff;         \
      if (v->data) {                                                           \
        v->data = realloc(v->data, new_capacity * sizeof(T));                  \
      } else {                                                                 \
        v->data = malloc(new_capacity * sizeof(T));                            \
      }                                                                        \
      v->capacity = new_capacity;                                              \
    }                                                                          \
  }                                                                            \
                                                                               \
  void vec_compact_##T(Vec_##T* v) {                                           \
    assert(v);                                                                 \
                                                                               \
    if (v->data && v->capacity > v->size) {                                    \
      v->data = realloc(v->data, v->size * sizeof(T));                         \
    }                                                                          \
    v->capacity = v->size;                                                     \
  }                                                                            \
                                                                               \
  ITEM_TYPE(T) vec_get_##T(Vec_##T* v, VEC_SIZE_T index) {                     \
    assert(v);                                                                 \
    assert(index >= 0);                                                        \
    assert(index < v->size);                                                   \
                                                                               \
    return ITEM_DEREF(v->data + index);                                        \
  }                                                                            \
                                                                               \
  void vec_set_##T(Vec_##T* v, VEC_SIZE_T index, ITEM_TYPE(T) item) {          \
    assert(v);                                                                 \
    assert(index >= 0);                                                        \
    assert(index < v->size);                                                   \
                                                                               \
    v->data[index] = ITEM_REF(item);                                           \
  }                                                                            \
                                                                               \
  void vec_push_##T(Vec_##T* v, ITEM_TYPE(T) item) {                           \
    assert(v);                                                                 \
                                                                               \
    vec_grow_##T(v, v->size + 1);                                              \
    v->size += 1;                                                              \
    vec_set_##T(v, v->size - 1, item);                                         \
  }                                                                            \
                                                                               \
  ITEM_TYPE(T) vec_pop_##T(Vec_##T* v) {                                       \
    assert(v);                                                                 \
    assert(v->size > 0);                                                       \
                                                                               \
    ITEM_TYPE(T) result = vec_get_##T(v, v->size - 1);                         \
    v->size -= 1;                                                              \
    return result;                                                             \
  }                                                                            \
                                                                               \
  void vec_insert_##T(Vec_##T* v, VEC_SIZE_T index, ITEM_TYPE(T) item) {       \
    assert(v);                                                                 \
    assert(index >= 0);                                                        \
    assert(index <= v->size);                                                  \
                                                                               \
    vec_grow_##T(v, v->size + 1);                                              \
    v->size += 1;                                                              \
    for (VEC_SIZE_T i = v->size - 1; i > index; --i) {                         \
      vec_set_##T(v, i, vec_get_##T(v, i - 1));                                \
    }                                                                          \
    vec_set_##T(v, index, item);                                               \
  }                                                                            \
                                                                               \
  void vec_insert_items_##T(Vec_##T* v, VEC_SIZE_T index, VEC_SIZE_T count,    \
                            T* items) {                                        \
    assert(v);                                                                 \
    assert(index >= 0);                                                        \
    assert(index <= v->size);                                                  \
                                                                               \
    vec_grow_##T(v, v->size + count);                                          \
    v->size += count;                                                          \
    for (VEC_SIZE_T i = v->size - 1; i > index; --i) {                         \
      vec_set_##T(v, i, vec_get_##T(v, i - count));                            \
    }                                                                          \
    for (VEC_SIZE_T o = 0; o < count; ++o) {                                   \
      vec_set_##T(v, index + o, *(items + o));                                 \
    }                                                                          \
  }                                                                            \
                                                                               \
  ITEM_TYPE(T) vec_remove_##T(Vec_##T* v, VEC_SIZE_T index) {                  \
    assert(v);                                                                 \
    assert(index >= 0);                                                        \
    assert(index < v->size);                                                   \
                                                                               \
    ITEM_TYPE(T) item = vec_get_##T(v, index);                                 \
    for (VEC_SIZE_T i = index; i < v->size - 1; ++i) {                         \
      vec_set_##T(v, i, vec_get_##T(v, i + 1));                                \
    }                                                                          \
    v->size -= 1;                                                              \
    return item;                                                               \
  }                                                                            \
                                                                               \
  void vec_foreach_##T(Vec_##T* v, void fn(ITEM_TYPE(T), VEC_SIZE_T)) {        \
    for (int i = 0; i < v->size; ++i) {                                        \
      fn(vec_get_##T(v, i), i);                                                \
    }                                                                          \
  }
