#ifndef DS_H
#define DS_H

#include <stdlib.h>
#include <string.h>

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

#endif // VECTOR_H

///////////////////////////////////////////////////////////////////////////////
//
// IMPLEMENTATIONS
//

#ifdef DS_IMPLEMENTATION

void vector_init(struct vector_t *vector) {
        vector->items = NULL;
        vector->sizes = NULL;
        vector->count = 0;
        vector->capacity = 0;
}

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

int vector_top(struct vector_t *vector, void *item, int size) {
        if (vector->count <= 0 || vector->sizes[vector->count - 1] != size) {
                return -1;
        }

        memcpy(item, vector->items[vector->count - 1], size);

        return 0;
}

int vector_pop(struct vector_t *vector) {
        if (vector->count <= 0) {
                return -1;
        }
        
        free(vector->items[vector->count - 1]);
        vector->count--;

        return 0;
}

int vector_get(struct vector_t *vector, int index, void *item, int size) {
        if (index < 0 || index >= vector->count || 
                        vector->sizes[vector->count - 1] != size) {
                return -1;
        }

        memcpy(item, vector->items[index], size);

        return 0;
}

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

void vector_delete(struct vector_t *vector) {
        free(vector->items);
        free(vector->sizes);

        vector->count = 0;
        vector->capacity = 0;
        vector->items = NULL;
        vector->sizes = NULL;
}

#endif // DS_IMPLEMENTATION

