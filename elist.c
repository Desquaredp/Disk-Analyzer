#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "logger.h"
#include "elist.h"

#define DEFAULT_INIT_SZ 10
#define RESIZE_MULTIPLIER 2

struct elist {
    size_t capacity;         /*!< Storage space allocated for list items */
    size_t size;             /*!< The actual number of items in the list */
    void **element_storage;  /*!< Pointer to the beginning of the array  */
};

bool idx_is_valid(struct elist *list, size_t idx);

struct elist *elist_create(size_t list_sz)
{   struct elist *list = malloc(sizeof(struct elist));
    if(list == NULL){
        return NULL;
    }

    if(list_sz == 0){
        list_sz = DEFAULT_INIT_SZ;
    }

    list->size =0;
    list->capacity = list_sz;
    list->element_storage = malloc(sizeof(void *) * list->capacity);
    if(list->element_storage == NULL){
        free(list);
        return NULL;
    }

    return list;
}

void elist_destroy(struct elist *list)
{
    free(list->element_storage);
    free(list);
    list->element_storage = NULL;
}

int elist_set_capacity(struct elist *list, size_t capacity)
{   
    if(capacity == 0){
        list->capacity =1;
        elist_clear(list);
    }

    if(capacity == list->capacity){
        return 0;
    }

    void* new_elements = NULL; 
    if(capacity !=0){
        new_elements = realloc(
                list->element_storage, sizeof(void *) * capacity);
    }else{
        new_elements = realloc(
                list->element_storage, sizeof(void *) * list->capacity);
    }
    if(new_elements == NULL){
        return -1;
    }
    list->element_storage = new_elements;
    if(capacity != 0){
        list->capacity = capacity;
    }
    if(list->capacity < list->size){
        list->size = list->capacity;
    }
    return 0;
}

size_t elist_capacity(struct elist *list)
{
    return list->capacity;
}

ssize_t elist_add(struct elist *list, void *item)
{

    if(list->size >= list->capacity){
        if(elist_set_capacity(
                    list, list->capacity * RESIZE_MULTIPLIER) == -1){
            return -1;
        }
    }

    size_t    index = list->size++;
    list->element_storage[index] = item;
    return index;
}

int elist_set(struct elist *list, size_t idx, void *item)
{   if(elist_get(list, idx) == NULL || idx >= list->size){
    return -1;
                                                         }

list->element_storage[idx] = item;
return 0;
}

void *elist_get(struct elist *list, size_t idx)
{     return list->element_storage[idx];
}

size_t elist_size(struct elist *list)
{
    return list->size;
}

int elist_remove(struct elist *list, size_t idx)
{
    if(idx >= list->size){
        return -1;
    }
    void * size = list->element_storage + 1;
    memmove(
            list->element_storage + idx,
            list->element_storage + idx + 1,
            (list->size - idx) *sizeof(size));


    list->size--;
    return 0;
}

void elist_clear(struct elist *list)
{   size_t size = list->size;
    for(int i = 0; i <  size ; i++){

        elist_remove(list,0);
    }
    list->size = 0;
}

void elist_clear_mem(struct elist *list)
{

    memset(list->element_storage, 0, (list->size) *sizeof(void *));
    list->size = 0;
}

ssize_t elist_index_of(struct elist *list, void *item, size_t item_sz)
{
    for(int i = 0; i < list->size; i++){

        if( memcmp(elist_get(list,i), item, item_sz) == 0){
            return i;
        }
    } 
    return -1;
}
void elist_sort(struct elist *list, int (*comparator)(const void *, const void *))
{
    qsort(list->element_storage, list->size, sizeof(void*), comparator);
}

bool idx_is_valid(struct elist *list, size_t idx)
{   
    if(idx >= list->size || idx < 0){
        return false;
    }
    return true;
}

