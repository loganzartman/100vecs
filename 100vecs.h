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

#define VEC_CLEANUP(T) __attribute__((__cleanup__(vec_cleanup_##T)))

#define VEC_DECL(T)                                                            \
  typedef struct Vec_##T Vec_##T;                                              \
  extern Vec_##T* vec_create_##T();                                            \
  extern void vec_delete_##T(Vec_##T* v);                                      \
  extern void vec_cleanup_##T(Vec_##T** v);                                    \
  extern VEC_SIZE_T vec_size_##T(Vec_##T* v);                                  \
  extern VEC_SIZE_T vec_capacity_##T(Vec_##T* v);                              \
  extern T* vec_data_##T(Vec_##T* v);                                          \
  extern Vec_##T* vec_clone_##T(Vec_##T* v);                                   \
  extern void vec_clear_##T(Vec_##T* v);                                       \
  extern void vec_grow_##T(Vec_##T* v, VEC_SIZE_T capacity);                   \
  extern void vec_compact_##T(Vec_##T* v);                                     \
  extern T vec_get_##T(Vec_##T* v, VEC_SIZE_T index);                          \
  extern void vec_set_##T(Vec_##T* v, VEC_SIZE_T index, T item);               \
  extern void vec_push_##T(Vec_##T* v, T item);                                \
  extern T vec_pop_##T(Vec_##T* v);                                            \
  extern void vec_insert_##T(Vec_##T* v, VEC_SIZE_T index, T item);            \
  extern void vec_insert_items_##T(Vec_##T* v, VEC_SIZE_T index,               \
                                   VEC_SIZE_T count, T* items);                \
  extern T vec_remove_##T(Vec_##T* v, VEC_SIZE_T index);                       \
  extern void vec_qsort_##T(Vec_##T* v,                                        \
                            int comparator(const T* a, const T* b));           \
  extern void vec_foreach_##T(Vec_##T* v, void fn(T, VEC_SIZE_T));

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
  Vec_##T* vec_clone_##T(Vec_##T* v) {                                         \
    assert(v);                                                                 \
                                                                               \
    Vec_##T* result = vec_create_##T();                                        \
    vec_grow_##T(result, v->size);                                             \
    memcpy(result->data, v->data, v->size * sizeof(T));                        \
    result->size = v->size;                                                    \
    return result;                                                             \
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
  T vec_get_##T(Vec_##T* v, VEC_SIZE_T index) {                                \
    assert(v);                                                                 \
    assert(index >= 0);                                                        \
    assert(index < v->size);                                                   \
                                                                               \
    return v->data[index];                                                     \
  }                                                                            \
                                                                               \
  void vec_set_##T(Vec_##T* v, VEC_SIZE_T index, T item) {                     \
    assert(v);                                                                 \
    assert(index >= 0);                                                        \
    assert(index < v->size);                                                   \
                                                                               \
    v->data[index] = item;                                                     \
  }                                                                            \
                                                                               \
  void vec_push_##T(Vec_##T* v, T item) {                                      \
    assert(v);                                                                 \
                                                                               \
    vec_grow_##T(v, v->size + 1);                                              \
    v->size += 1;                                                              \
    vec_set_##T(v, v->size - 1, item);                                         \
  }                                                                            \
                                                                               \
  T vec_pop_##T(Vec_##T* v) {                                                  \
    assert(v);                                                                 \
    assert(v->size > 0);                                                       \
                                                                               \
    T result = vec_get_##T(v, v->size - 1);                                    \
    v->size -= 1;                                                              \
    return result;                                                             \
  }                                                                            \
                                                                               \
  void vec_insert_##T(Vec_##T* v, VEC_SIZE_T index, T item) {                  \
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
  T vec_remove_##T(Vec_##T* v, VEC_SIZE_T index) {                             \
    assert(v);                                                                 \
    assert(index >= 0);                                                        \
    assert(index < v->size);                                                   \
                                                                               \
    T item = vec_get_##T(v, index);                                            \
    for (VEC_SIZE_T i = index; i < v->size - 1; ++i) {                         \
      vec_set_##T(v, i, vec_get_##T(v, i + 1));                                \
    }                                                                          \
    v->size -= 1;                                                              \
    return item;                                                               \
  }                                                                            \
                                                                               \
  void vec_foreach_##T(Vec_##T* v, void fn(T, VEC_SIZE_T)) {                   \
    for (int i = 0; i < v->size; ++i) {                                        \
      fn(vec_get_##T(v, i), i);                                                \
    }                                                                          \
  }                                                                            \
                                                                               \
  void vec_qsort_##T(Vec_##T* v, int comparator(const T* a, const T* b)) {     \
    qsort(v->data, v->size, sizeof(T),                                         \
          (int (*)(const void*, const void*))comparator);                      \
  }
