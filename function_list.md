## vecs
```C
typedef struct Vec_T Vec_T;
uint32_t vec_capacity_T(Vec_T* v);
void vec_cleanup_T(Vec_T** v);
void vec_clear_T(Vec_T* v);
Vec_T* vec_clone_T(Vec_T* v);
void vec_compact_T(Vec_T* v);
Vec_T* vec_create_T();
T* vec_data_T(Vec_T* v);
void vec_destroy_T(Vec_T* v);
void vec_foreach_T(Vec_T* v, void fn(T, uint32_t));
T vec_get_T(Vec_T* v, uint32_t index);
void vec_grow_T(Vec_T* v, uint32_t capacity);
void vec_insert_T(Vec_T* v, uint32_t index, T item);
void vec_insert_items_T(Vec_T* v, uint32_t index, uint32_t count, T* items);
T vec_pop_T(Vec_T* v);
void vec_push_T(Vec_T* v, T item);
void vec_qsort_T(Vec_T* v, int comparator(const T* a, const T* b));
T vec_remove_T(Vec_T* v, uint32_t index);
void vec_set_T(Vec_T* v, uint32_t index, T item);
uint32_t vec_size_T(Vec_T* v);
```

## maps
```C
map_delete_K__V(Map_K__V* m, K key);
map_has_K__V(Map_K__V* m, K key);
typedef struct MapEntry_K__V MapEntry_K__V;
typedef struct Map_K__V Map_K__V;
uint32_t map_capacity_K__V(Map_K__V* m);
Map_K__V* map_create_K__V(int hash(K key));
MapEntry_K__V* map_data_K__V(Map_K__V* m);
void map_destroy_K__V(Map_K__V* m);
V* map_get_K__V(Map_K__V* m, K key);
V map_get_else_K__V(Map_K__V* m, K key, V else_);
void map_put_K__V(Map_K__V* m, K key, V value);
void map_put_unsafe_K__V(Map_K__V* m, MapEntry_K__V entry);
void map_resize_K__V(Map_K__V* m);
uint32_t map_size_K__V(Map_K__V* m);
```
