#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//  888  888  .d88b.   .d8888b .d8888b
//  888  888 d8P  Y8b d88P"    88K
//  Y88  88P 88888888 888      "Y8888b.
//   Y8bd8P  Y8b.     Y88b.         X88
//    Y88P    "Y8888   "Y8888P  88888P'

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

//  88888b.d88b.   8888b.  88888b.  .d8888b
//  888 "888 "88b     "88b 888 "88b 88K
//  888  888  888 .d888888 888  888 "Y8888b.
//  888  888  888 888  888 888 d88P      X88
//  888  888  888 "Y888888 88888P"   88888P'
//                         888
//                         888

#if !defined(MAP_DEFAULT_CAPACITY)
#define MAP_DEFAULT_CAPACITY 10
#endif

#if !defined(MAP_MAX_LOAD)
#define MAP_MAX_LOAD 0.6
#endif

#define MAP_MIN_LOAD (MAP_MAX_LOAD * 0.25)
#define MAP_GROW_FACTOR 2.0
#define MAP_SHRINK_FACTOR 0.5

#define MAP_DECL(K, V)                                                         \
  typedef struct MapEntry_##K##__##V MapEntry_##K##__##V;                      \
  typedef struct Map_##K##__##V Map_##K##__##V;                                \
  extern VEC_SIZE_T map_size_##K##__##V(Map_##K##__##V* m);                    \
  extern VEC_SIZE_T map_capacity_##K##__##V(Map_##K##__##V* m);                \
  extern MapEntry_##K##__##V* map_data_##K##__##V(Map_##K##__##V* m);          \
  extern Map_##K##__##V* map_create_##K##__##V(int hash(K key));               \
  extern void map_delete_##K##__##V(Map_##K##__##V* m);                        \
  extern void map_put_unsafe_##K##__##V(Map_##K##__##V* m,                     \
                                        MapEntry_##K##__##V entry);            \
  extern void map_resize_##K##__##V(Map_##K##__##V* m);                        \
  extern V* map_get_##K##__##V(Map_##K##__##V* map, K key);                    \
  extern V map_get_else_##K##__##V(Map_##K##__##V* map, K key, V else_);       \
  extern bool map_has_##K##__##V(Map_##K##__##V* m, K key);                    \
  extern void map_put_##K##__##V(Map_##K##__##V* map, K key, V value);

#define MAP_IMPL(K, V)                                                         \
  typedef struct MapEntry_##K##__##V {                                         \
    K key;                                                                     \
    V value;                                                                   \
  } MapEntry_##K##__##V;                                                       \
                                                                               \
  typedef struct Map_##K##__##V {                                              \
    VEC_SIZE_T size;                                                           \
    VEC_SIZE_T capacity;                                                       \
    int (*hash)(K key);                                                        \
    MapEntry_##K##__##V* data;                                                 \
    bool* data_present;                                                        \
  } Map_##K##__##V;                                                            \
                                                                               \
  VEC_SIZE_T map_size_##K##__##V(Map_##K##__##V* m) {                          \
    return m->size;                                                            \
  }                                                                            \
                                                                               \
  VEC_SIZE_T map_capacity_##K##__##V(Map_##K##__##V* m) {                      \
    return m->capacity;                                                        \
  };                                                                           \
                                                                               \
  MapEntry_##K##__##V* map_data_##K##__##V(Map_##K##__##V* m) {                \
    return m->data;                                                            \
  };                                                                           \
                                                                               \
  Map_##K##__##V* map_create_##K##__##V(int hash(K key)) {                     \
    Map_##K##__##V* m = malloc(sizeof(Map_##K##__##V));                        \
    m->size = 0;                                                               \
    m->capacity = 0;                                                           \
    m->hash = hash;                                                            \
    m->data = NULL;                                                            \
    return m;                                                                  \
  }                                                                            \
                                                                               \
  void map_delete_##K##__##V(Map_##K##__##V* m) {                              \
    if (m->data) {                                                             \
      free(m->data);                                                           \
      m->data = NULL;                                                          \
    }                                                                          \
    free(m);                                                                   \
  }                                                                            \
                                                                               \
  void map_put_unsafe_##K##__##V(Map_##K##__##V* m,                            \
                                 MapEntry_##K##__##V entry) {                  \
    int h = m->hash(entry.key);                                                \
    int i0 = h % m->capacity;                                                  \
    int i = i0;                                                                \
    while (m->data_present[i]) {                                               \
      i = (i + 1) % m->capacity;                                               \
      assert(i != i0);                                                         \
    }                                                                          \
    m->data[i] = entry;                                                        \
    m->data_present[i] = true;                                                 \
    m->size += 1;                                                              \
  }                                                                            \
                                                                               \
  void map_resize_##K##__##V(Map_##K##__##V* m) {                              \
    if (m->data) {                                                             \
      float load = (float)m->size / m->capacity;                               \
      if (load > MAP_MAX_LOAD || load < MAP_MIN_LOAD) {                        \
        VEC_SIZE_T old_capacity = m->capacity;                                 \
        MapEntry_##K##__##V* old_data = m->data;                               \
        bool* old_data_present = m->data_present;                              \
                                                                               \
        if (load > MAP_MAX_LOAD) {                                             \
          m->capacity = (VEC_SIZE_T)(m->capacity * MAP_GROW_FACTOR);           \
        } else {                                                               \
          m->capacity = (VEC_SIZE_T)(m->capacity * MAP_SHRINK_FACTOR);         \
        }                                                                      \
                                                                               \
        m->size = 0;                                                           \
        m->data = malloc(m->capacity * sizeof(MapEntry_##K##__##V));           \
        m->data_present = calloc(m->capacity, sizeof(bool));                   \
                                                                               \
        for (int i = 0; i < old_capacity; ++i) {                               \
          if (old_data_present[i]) {                                           \
            map_put_unsafe_##K##__##V(m, old_data[i]);                         \
          }                                                                    \
        }                                                                      \
                                                                               \
        free(old_data);                                                        \
        free(old_data_present);                                                \
      }                                                                        \
    } else {                                                                   \
      m->capacity = MAP_DEFAULT_CAPACITY;                                      \
      m->data = malloc(m->capacity * sizeof(V));                               \
      m->data_present = calloc(m->capacity, sizeof(bool));                     \
    }                                                                          \
  }                                                                            \
                                                                               \
  V* map_get_##K##__##V(Map_##K##__##V* m, K key) {                            \
    int h = m->hash(key);                                                      \
    int i0 = h % m->capacity;                                                  \
    int i = i0;                                                                \
    while (m->data_present[i]) {                                               \
      if (m->data[i].key == key) {                                             \
        return &m->data[i].value;                                              \
      }                                                                        \
      i = (i + 1) % m->capacity;                                               \
      assert(i != i0);                                                         \
    }                                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  V map_get_else_##K##__##V(Map_##K##__##V* m, K key, V else_) {               \
    V* maybe_v = map_get_##K##__##V(m, key);                                   \
    if (!maybe_v) {                                                            \
      return else_;                                                            \
    }                                                                          \
    return *maybe_v;                                                           \
  }                                                                            \
                                                                               \
  bool map_has_##K##__##V(Map_##K##__##V* m, K key) {                          \
    return map_get_##K##__##V(m, key) != NULL;                                 \
  }                                                                            \
                                                                               \
  void map_put_##K##__##V(Map_##K##__##V* m, K key, V value) {                 \
    map_resize_##K##__##V(m);                                                  \
    map_put_unsafe_##K##__##V(                                                 \
        m, (MapEntry_##K##__##V){.key = key, .value = value});                 \
  }
