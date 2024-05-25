#include <stdio.h>

#define DS_IMPLEMENTATION
#include "ds.h"

int main() {
        struct vector_t vector;

        // initializing the vector
        vector_init(&vector);

        // insert some values into vector
        for (int i = 0; i < 10; i++) {
                vector_append(&vector, &i, sizeof(i));

                // get the top value
                int top;
                vector_top(&vector, &top, sizeof(top));
                printf("Top: %d\n", top);
        }

        // get each index values
        for (int i = 0; i < 10; i++) {
                int value;
                vector_get(&vector, i, &value, sizeof(value));
                printf("%d-th value: %d\n", i, value);
        }

        // pop out values
        for (int i = 0; i < 10; i++) {
                int top;
                vector_top(&vector, &top, sizeof(top));
                printf("Top: %d\n", top);

                vector_pop(&vector);
        }

        // insert some values into vector
        for (int i = 0; i < 10; i++) {
                vector_append(&vector, &i, sizeof(i));

                // get the top value
                int top;
                vector_top(&vector, &top, sizeof(top));
                printf("Top: %d\n", top);
        }

        // set random values
        for (int i = 0; i < 10; i++) {
                int value = rand() % 100;
                printf("setting %d-th value: %d\n", i, value);
                vector_set(&vector, i, &value, sizeof(value));
                vector_get(&vector, i, &value, sizeof(value));
                printf("%d-th value: %d\n", i, value);
        }

        return 0;
}
