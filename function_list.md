```C
typedef struct Vec_T Vec_T;
uint32_t vec_capacity_T(Vec_T* v);
void vec_cleanup_T(Vec_T** v);
void vec_clear_T(Vec_T* v);
void vec_compact_T(Vec_T* v);
Vec_T* vec_create_T();
T* vec_data_T(Vec_T* v);
void vec_delete_T(Vec_T* v);
void vec_foreach_T(Vec_T* v, void fn(T, uint32_t));
T vec_get_T(Vec_T* v, uint32_t index);
void vec_grow_T(Vec_T* v, uint32_t capacity);
void vec_insert_T(Vec_T* v, uint32_t index, T item);
void vec_insert_items_T(Vec_T* v, uint32_t index, uint32_t count, T* items);
T vec_pop_T(Vec_T* v);
void vec_push_T(Vec_T* v, T item);
T vec_remove_T(Vec_T* v, uint32_t index);
void vec_set_T(Vec_T* v, uint32_t index, T item);
uint32_t vec_size_T(Vec_T* v);
```
