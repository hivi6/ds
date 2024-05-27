#ifndef DS_H
#define DS_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

enum ds_error_enum {
        DS_NO_ERROR = 0,        // when there is no error
        DS_ARGUMENT_ERROR,      // errors related to function arguments
        DS_MALLOC_ERROR,        // error related to broken malloc
        DS_EMPTY_ERROR,         // accessing elements when there is nothing
        DS_SIZE_ERROR,          // argument size doesnot match with stored size
        DS_RANGE_ERROR,         // index out of range error
        DS_NUM_OF_ERRORS        // this is not an error
};

void ds_print_error(enum ds_error_enum);

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

int vector_init(struct vector_t *vector);
int vector_append(struct vector_t *vector, void *item, int size);
int vector_top(struct vector_t *vector, void *item, int size);
int vector_pop(struct vector_t *vector);
int vector_get(struct vector_t *vector, int index, void *item, int size);
int vector_set(struct vector_t *vector, int index, void *item, int size);
int vector_delete(struct vector_t *vector);

// string builder
//
// string builder helps you to build strings
// it uses the vector type to grow dynamically
struct string_builder_t {
        struct vector_t chars;
};

int string_builder_init(struct string_builder_t *sb);
int string_builder_appendf(struct string_builder_t *sb, const char *format, 
                          ...);
int string_builder_appendn(struct string_builder_t *sb, const char *str,
                           int len);
int string_builder_append(struct string_builder_t *sb, const char *str);
int string_builder_appendc(struct string_builder_t *sb, char ch);
int string_builder_appendcn(struct string_builder_t *sb, char ch, int len);
int string_builder_get(struct string_builder_t *sb, int index, char *ch);
int string_builder_set(struct string_builder_t *sb, int index, char ch);
int string_builder_build(struct string_builder_t *sb, char **str);
int string_builder_delete(struct string_builder_t *sb);

#endif // DH_H

///////////////////////////////////////////////////////////////////////////////
//
// IMPLEMENTATIONS
//

#ifdef DS_IMPLEMENTATION

void ds_print_error(enum ds_error_enum err) {
        const char *error_msgs[DS_NUM_OF_ERRORS] = {
                "no error",
                "invalid argument",
                "invalid malloc result",
                "no items",
                "invalid size",
                "out of range",
        };
        if (err >= DS_NUM_OF_ERRORS) {
                printf("invalid error no\n");
                return;
        }
        printf("%s", error_msgs[err]);
}

// initialize the vector
//
// keeps count, capacity as 0
// keeps items, sizes as NULL
// returns 0 if no error
int vector_init(struct vector_t *vector) {
        if (vector == NULL) {
                return DS_ARGUMENT_ERROR;
        }
        vector->items = NULL;
        vector->sizes = NULL;
        vector->count = 0;
        vector->capacity = 0;
        return DS_NO_ERROR;
}

// append an item to the vector
//
// returns 0 if no error
// allocates 8 value at a time if max capacity is reached
int vector_append(struct vector_t *vector, void *item, int size) {
        if (vector == NULL || item == NULL) {
                return DS_ARGUMENT_ERROR;
        }

        if (vector->count >= vector->capacity) {
                int new_capacity = vector->capacity + 8;
                void **temp_items = malloc(new_capacity * sizeof(void *));
                int   *temp_sizes = malloc(new_capacity * sizeof(int));
                
                if (temp_items == NULL || temp_sizes == NULL) {
                        return DS_MALLOC_ERROR;
                }

                for (int i = 0; i < vector->count; i++) {
                        temp_items[i] = vector->items[i];
                        temp_sizes[i] = vector->sizes[i];
                }

                free(vector->items);
                free(vector->sizes);

                vector->items = temp_items;
                vector->sizes = temp_sizes;
                vector->capacity = new_capacity;
        }

        char *temp = malloc(size * sizeof(char));
        if (temp == NULL) {
                return DS_MALLOC_ERROR;
        }
        memcpy(temp, item, size);

        vector->sizes[vector->count] = size;
        vector->items[vector->count] = temp;
        vector->count++;

        return DS_NO_ERROR;
}

// get the top value of the vector
//
// returns 0 if no error
int vector_top(struct vector_t *vector, void *item, int size) {
        if (vector == NULL || item == NULL) {
                return DS_ARGUMENT_ERROR;
        }
        if (vector->count <= 0) {
                return DS_EMPTY_ERROR;
        }
        if (vector->sizes[vector->count - 1] != size) {
                return DS_SIZE_ERROR;
        }

        memcpy(item, vector->items[vector->count - 1], size);

        return DS_NO_ERROR;
}

// removes the last value of the vector
//
// returns 0 if no error
int vector_pop(struct vector_t *vector) {
        if (vector == NULL) {
                return DS_ARGUMENT_ERROR;
        }
        if (vector->count <= 0) {
                return DS_EMPTY_ERROR;
        }
        
        free(vector->items[vector->count - 1]);
        vector->count--;

        return DS_NO_ERROR;
}

// get the value at a specific index
//
// returns 0 if no error
int vector_get(struct vector_t *vector, int index, void *item, int size) {
        if (vector == NULL || item == NULL) {
                return DS_ARGUMENT_ERROR;
        }
        if (index < 0 || index >= vector->count) {
                return DS_RANGE_ERROR;
        }
        if (vector->sizes[vector->count - 1] != size) {
                return DS_SIZE_ERROR;
        }

        memcpy(item, vector->items[index], size);

        return DS_NO_ERROR;
}

// set the value at a specific index
//
// returns 0 if no error
int vector_set(struct vector_t *vector, int index, void *item, int size) {
        if (vector == NULL || item == NULL) {
                return DS_ARGUMENT_ERROR;
        }
        if (index < 0 || index >= vector->count) {
                return DS_RANGE_ERROR;
        }

        free(vector->items[index]);
        char *temp = malloc(size * sizeof(char));
        if (temp == NULL) {
                return DS_MALLOC_ERROR;
        }

        vector->items[index] = temp;
        vector->sizes[index] = size;
        memcpy(vector->items[index], item, size);

        return DS_NO_ERROR;
}

// delete the vector
//
// frees the item space that was allocated
// set all the fields to zero
int vector_delete(struct vector_t *vector) {
        if (vector == NULL) {
                return DS_ARGUMENT_ERROR;
        }

        for (int i = 0; i < vector->count; i++) {
                free(vector->items[i]);
        }
        free(vector->items);
        free(vector->sizes);

        vector->count = 0;
        vector->capacity = 0;
        vector->items = NULL;
        vector->sizes = NULL;

        return DS_NO_ERROR;
}

// initialize a string builder
//
// it uses vector under the hood
int string_builder_init(struct string_builder_t *sb) {
        if (sb == NULL) {
                return DS_ARGUMENT_ERROR;
        }

        return vector_init(&sb->chars);
}

// append a formated string like printf
//
// uses vsnprintf to calculate the final string
// and then allocate the required memory
// and append the final string to the string_builder
// returns 0 if nothing went wrong while appending the string
int string_builder_appendf(struct string_builder_t *sb, const char *format,
                          ...) {
        if (sb == NULL) {
                return DS_ARGUMENT_ERROR;
        }

        va_list args;
        va_start(args, format);
        int needed = vsnprintf(NULL, 0, format, args);
        va_end(args);

        char *buffer = malloc((needed + 1) * sizeof(char));
        if (buffer == NULL) {
                return DS_MALLOC_ERROR;
        }

        va_start(args, format);
        vsnprintf(buffer, needed + 1, format, args);
        va_end(args);

        for (int i = 0; i < needed + 1; i++) {
                int err = vector_append(&sb->chars, (void *)&buffer[i], 
                                        sizeof(char));
                if (err) {
                        return err;
                }
        }
        free(buffer);

        return DS_NO_ERROR;
}

// append a given string n times
//
// returns 0 if nothing went wrong
// returns -1 if something went wrong
int string_builder_appendn(struct string_builder_t *sb, const char *str,
                           int len) {
        if (sb == NULL || str == NULL) {
                return DS_ARGUMENT_ERROR;
        }

        for (int i = 0; i < len; i++) {
                for (int j = 0; str[j]; j++) {
                        int err = vector_append(&sb->chars, (void*)&str[j], 
                                                sizeof(char));
                        if (err) {
                                return err;
                        }
                }
        }

        return DS_NO_ERROR;
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
int string_builder_appendc(struct string_builder_t *sb, char ch) {
        return vector_append(&sb->chars, (void*)&ch, sizeof(char));
}

// append a given character n times
//
// returns 0 if nothing went wrong
int string_builder_appendcn(struct string_builder_t *sb, char ch, int len) {
        if (sb == NULL) {
                return DS_ARGUMENT_ERROR;
        }

        for (int i = 0; i < len; i++) {
                int err = vector_append(&sb->chars, (void*)&ch, sizeof(char));
                if (err) {
                        return err;
                }
        }
        return DS_NO_ERROR;
}

// build the string and allocate the string to the provided pointer
//
// returns 0 if nothing goes wrong
int string_builder_build(struct string_builder_t *sb, char **str) {
        if (sb == NULL || str == NULL) { 
                return DS_ARGUMENT_ERROR;
        }

        *str = malloc(sizeof(char) * sb->chars.count + 1);
        if (str == NULL) {
                return DS_MALLOC_ERROR;
        }

        for (int i = 0; i < sb->chars.count; i++) {
                int err = vector_get(&sb->chars, i, *str + i, sizeof(char));
                if (err) {
                        return err;
                }
        }
        (*str)[sb->chars.count] = 0;

        return 0;
}

// delete the string builder
int string_builder_delete(struct string_builder_t *sb) {
        if (sb == NULL) {
                return DS_ARGUMENT_ERROR;
        }
        return vector_delete(&sb->chars);
}

// get the character at a given index
// 
// returns 0 if nothing goes wrong
int string_builder_get(struct string_builder_t *sb, int index, char *ch) {
        if (sb == NULL || ch == NULL) {
                return DS_ARGUMENT_ERROR;
        }
        return vector_get(&sb->chars, index, (void*)&ch, sizeof(char));
}

// set the character at a given index
//
// returns -1 if something went wrong
int string_builder_set(struct string_builder_t *sb, int index, char ch) {
        if (sb == NULL) {
                return DS_ARGUMENT_ERROR;
        }
        return vector_set(&sb->chars, index, (void*)&ch, sizeof(ch));
}

#endif // DS_IMPLEMENTATION

