#include "rpc.h"
#include "crc32.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

char* rpc_header_encode(char *request)
{
    char *header = (char *)malloc(RPC_HEADER_LENGTH);

    unsigned int crc = calc_crc32(request, strlen(request));
    unsigned short str_length = strlen(request);
    memcpy(header, &crc, 4);
    memcpy(header + 4, &str_length, 2);

    return header;
}