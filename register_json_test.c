#include "rpc.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./%s <filename>\n", argv[0]);
        return -1;
    }

    char *filename = argv[1];
    char *json = rpc_read_register_config(filename);
    printf("register json: %s\n", json);
    rpc_decode_register_json(json);

    struct rpc_func *func = rpc_caller_table;
    if (!func) {
        printf("no function\n");
        return 0;
    }
    while (func) {
        printf("%s ", func->rettype);
        printf("%s", func->method);
        printf("(");
        int i = 0;
        for (i = 0; i < func->count; i++) {
            printf("%s %s", func->types[i], func->params[i]);
            if (i < func->count - 1) {
                printf(",");
            }
        }
        printf(")\n");
        func = func->next;
    }
    
    return 0;
}