#ifndef DS_H
#define DS_H

#include <stdlib.h>
#include <string.h>

// vector
//
// vector is an array that grows dynamically
// implementation wise the array grows 8 items at a time
// this is a generic implementation and can be used with any type
struct vector_t {
        void **items;
        int  *sizes;
        int  count;
        int  capacity;
};

void vector_init(struct vector_t *vector);
int  vector_append(struct vector_t *vector, void *item, int size);
int  vector_top(struct vector_t *vector, void *item, int size);
int  vector_pop(struct vector_t *vector);
int  vector_get(struct vector_t *vector, int index, void *item, int size);
int  vector_set(struct vector_t *vector, int index, void *item, int size);
void vector_delete(struct vector_t *vector);

#endif // DH_H

///////////////////////////////////////////////////////////////////////////////
//
// IMPLEMENTATIONS
//

#ifdef DS_IMPLEMENTATION

// initialize the vector
//
// keeps count, capacity as 0
// keeps items, sizes as NULL
void vector_init(struct vector_t *vector) {
        vector->items = NULL;
        vector->sizes = NULL;
        vector->count = 0;
        vector->capacity = 0;
}

// append an item to the vector
//
// returns 0 if item was successfully appended
// returns -1 if something went wrong while allocating memory
// allocates 8 value at a time if max capacity is reached
int vector_append(struct vector_t *vector, void *item, int size) {
        if (vector->count >= vector->capacity) {
                vector->capacity += 8;
                void **temp_items = malloc(vector->capacity * sizeof(void *));
                int   *temp_sizes = malloc(vector->capacity * sizeof(int));
                
                if (temp_items == NULL || temp_sizes == NULL) {
                        return -1;
                }

                for (int i = 0; i < vector->count; i++) {
                        temp_items[i] = vector->items[i];
                        temp_sizes[i] = vector->sizes[i];
                }
                free(vector->items);
                free(vector->sizes);
                vector->items = temp_items;
                vector->sizes = temp_sizes;
        }

        vector->sizes[vector->count] = size;
        vector->items[vector->count] = malloc(size * sizeof(char));
        memcpy(vector->items[vector->count], item, size);

        vector->count++;

        return 0;
}

// get the top value of the vector
//
// returns 0 if the top value exists
// returns -1 if the top value doesnot exists, i.e. no value exists
int vector_top(struct vector_t *vector, void *item, int size) {
        if (vector->count <= 0 || vector->sizes[vector->count - 1] != size) {
                return -1;
        }

        memcpy(item, vector->items[vector->count - 1], size);

        return 0;
}

// removes the last value of the vector
//
// returns 0 if the last value was removed
// returns -1 if last value was not found
int vector_pop(struct vector_t *vector) {
        if (vector->count <= 0) {
                return -1;
        }
        
        free(vector->items[vector->count - 1]);
        vector->count--;

        return 0;
}

// get the value at a specific index
//
// returns 0 if index is within the valid range, i.e. 0 <= index < items.count
// returns -1 if index is outside the valid range
int vector_get(struct vector_t *vector, int index, void *item, int size) {
        if (index < 0 || index >= vector->count || 
                        vector->sizes[vector->count - 1] != size) {
                return -1;
        }

        memcpy(item, vector->items[index], size);

        return 0;
}

// set the value at a specific index
//
// returns 0 if index is within the valid range, i.e. 0 <= index < items.count
// returns -1 if index is outside the valid range
int vector_set(struct vector_t *vector, int index, void *item, int size) {
        if (index < 0 || index >= vector->count) {
                return -1;
        }

        free(vector->items[index]);
        vector->items[index] = malloc(size * sizeof(char));
        vector->sizes[index] = size;
        memcpy(vector->items[index], item, size);

        return 0;
}

// delete the vector
//
// frees the item space that was allocated
// set all the fields to zero
void vector_delete(struct vector_t *vector) {
        free(vector->items);
        free(vector->sizes);

        vector->count = 0;
        vector->capacity = 0;
        vector->items = NULL;
        vector->sizes = NULL;
}

#endif // DS_IMPLEMENTATION

