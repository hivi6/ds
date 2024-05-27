#ifndef DS_H
#define DS_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

// string builder
//
// string builder helps you to build strings
// it uses the vector type to grow dynamically
struct string_builder_t {
        struct vector_t chars;
};

void string_builder_init(struct string_builder_t *sb);
int string_builder_appendf(struct string_builder_t *sb, const char *format, 
                          ...);
int string_builder_appendn(struct string_builder_t *sb, const char *str,
                           int len);
int string_builder_append(struct string_builder_t *sb, const char *str);
int string_builder_appendc(struct string_builder_t *sb, char ch);
int string_builder_appendcn(struct string_builder_t *sb, char ch, int len);
char string_builder_get(struct string_builder_t *sb, int index);
int string_builder_set(struct string_builder_t *sb, int index, char ch);
int string_builder_build(struct string_builder_t *sb, char **str);
void string_builder_delete(struct string_builder_t *sb);

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
        for (int i = 0; i < vector->count; i++) {
                free(vector->items[i]);
        }
        free(vector->items);
        free(vector->sizes);

        vector->count = 0;
        vector->capacity = 0;
        vector->items = NULL;
        vector->sizes = NULL;
}

// initialize a string builder
//
// it uses vector under the hood
void string_builder_init(struct string_builder_t *sb) {
        vector_init(&sb->chars);
}

// append a formated string like printf
//
// uses vsnprintf to calculate the final string
// and then allocate the required memory
// and append the final string to the string_builder
// returns 0 if nothing went wrong while appending the string
// returns -1 if something went wrong
int string_builder_appendf(struct string_builder_t *sb, const char *format,
                          ...) {
        va_list args;
        va_start(args, format);
        int needed = vsnprintf(NULL, 0, format, args);
        va_end(args);

        char *buffer = malloc((needed + 1) * sizeof(char));
        if (buffer == NULL) {
                return -1;
        }

        va_start(args, format);
        vsnprintf(buffer, needed + 1, format, args);
        va_end(args);

        for (int i = 0; i < needed + 1; i++) {
                vector_append(&sb->chars, (void *)&buffer[i], sizeof(char));
        }
        free(buffer);

        return 0;
}

// append a given string n times
//
// returns 0 if nothing went wrong
// returns -1 if something went wrong
int string_builder_appendn(struct string_builder_t *sb, const char *str,
                           int len) {
        for (int i = 0; i < len; i++) {
                for (int j = 0; str[j]; j++) {
                        vector_append(&sb->chars, (void*)&str[j], sizeof(char));
                }
        }
        return 0;
}

// append a given string
//
// returns 0 if nothing went wrong
int string_builder_append(struct string_builder_t *sb, const char *str) {
        return string_builder_appendn(sb, str, 1);
}

// append a given character
// 
// return 0 if nothing went wrong
// return -1 if something went wrong
int string_builder_appendc(struct string_builder_t *sb, char ch) {
        vector_append(&sb->chars, &ch, sizeof(char));
        return 0;
}

// append a given character n times
//
// returns 0 if nothing went wrong
int string_builder_appendcn(struct string_builder_t *sb, char ch, int len) {
        for (int i = 0; i < len; i++) {
                vector_append(&sb->chars, &ch, sizeof(char));
        }
        return 0;
}

// build the string and allocate the string to the provided pointer
//
// returns 0 if nothing goes wrong
int string_builder_build(struct string_builder_t *sb, char **str) {
        *str = malloc(sizeof(char) * sb->chars.count + 1);
        if (str == NULL) {
                return -1;
        }

        for (int i = 0; i < sb->chars.count; i++) {
                vector_get(&sb->chars, i, *str + i, sizeof(char));
        }
        (*str)[sb->chars.count] = 0;

        return 0;
}

// delete the string builder
void string_builder_delete(struct string_builder_t *sb) {
        vector_delete(&sb->chars);
}

// get the character at a given index
// 
// returns character value at a given index
// returns 0 if something went wrong
char string_builder_get(struct string_builder_t *sb, int index) {
        char ch;
        if (vector_get(&sb->chars, index, (void*)&ch, sizeof(ch))) {
                return 0;
        }
        return ch;
}

// set the character at a given index
//
// returns -1 if something went wrong
int string_builder_set(struct string_builder_t *sb, int index, char ch) {
        return vector_set(&sb->chars, index, (void*)&ch, sizeof(ch));
}

#endif // DS_IMPLEMENTATION

