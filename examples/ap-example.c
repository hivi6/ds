#define DS_IMPLEMENTATION
#include "ds.h"

int main(int argc, const char **argv) {
        struct ap_argument_t help, verbose, output;
        
        ap_argument_init(&help, AP_FLAG, "-h", "--help", "show help");
        ap_argument_init(&verbose, AP_FLAG, "-v", "--verbose", "show more");
        ap_argument_init(&output, AP_FVALUE, "-o", "--output", "output file");

        struct ap_parser_t parser;

        ap_parser_init(&parser, "ap-example", "0.1.0", "This is an example");

        ap_parser_add_argument(&parser, &help);
        ap_parser_add_argument(&parser, &verbose);
        ap_parser_add_argument(&parser, &output);

        int err = ap_parser_parse(&parser, argc, argv);
        if (err) {
                printf("Error: ");
                ds_print_error(err);
                ap_parser_usage(&parser);
                return 1;
        }

        printf("help: %d %s\n", help.is_exists, help.value);
        printf("verbose: %d %s\n", verbose.is_exists, verbose.value);
        printf("output: %d %s\n", output.is_exists, output.value);

        ap_parser_delete(&parser);

        ap_argument_delete(&help);
        ap_argument_delete(&verbose);
        ap_argument_delete(&output);

        return 0;
}
