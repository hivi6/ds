#include <stdio.h>

#define DS_IMPLEMENTATION
#include "ds.h"

int main() {
        struct string_builder_t sb;

        // initialize
        string_builder_init(&sb);

        // append some string
        string_builder_appendn(&sb, "abc-xyz ", 3);

        // append some more stirng
        string_builder_appendc(&sb, '(');
        string_builder_appendc(&sb, ')');
        string_builder_appendc(&sb, ' ');
        
        // append some moree string
        string_builder_appendcn(&sb, '0', 10);

        // append some moreee string
        string_builder_append(&sb, " abc-%d", 1000);

        // get the character at a given index
        printf("0th index: %c\n", string_builder_get(&sb, 0));

        // set the character at a given index
        printf("Changing 0th index to ' '\n");
        string_builder_set(&sb, 0, ' ');

        // build the string
        char *res;
        string_builder_build(&sb, &res);
        printf("Result: '%s'\n", res);
        free(res);

        string_builder_delete(&sb);

        return 0;
}
