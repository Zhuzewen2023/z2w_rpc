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
    rpc_free(json);
    return 0;
}