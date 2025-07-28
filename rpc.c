#define _GNU_SOURCE

#include "rpc.h"
#include "crc32.h"
#include "cJSON.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <link.h>
#include <dlfcn.h>

char *sayhello_request = "{ \n \
    \"method\" : \"sayhello\",\n \
    \"param\" : {\n \
        \"msg\" : \"zrpc nb\",\n \
        \"length\" : 7\n \
    },\n \
    \"callerid\" : 12345\n \
}";

char *add_request = "{ \n \
    \"method\" : \"add\",\n \
    \"param\" : {\n \
        \"a\" : 10,\n \
        \"b\" : 7\n \
    },\n \
    \"callerid\" : 12346\n \
}";

char *sub_request = "{ \n \
    \"method\" : \"sub\",\n \
    \"param\" : {\n \
        \"a\" : 6.7,\n \
        \"b\" : 3.4\n \
    },\n \
    \"callerid\" : 12347\n \
}";

char *mul_request = "{ \n \
    \"method\" : \"mul\",\n \
    \"param\" : {\n \
        \"a\" : 2.4,\n \
        \"b\" : 3.6\n \
    },\n \
    \"callerid\" : 12348\n \
}";

char *sayhello_response = "{ \n \
	\"method\" : \"sayhello\",\n \
	\"result\" : \"bn cprz\",\n \
	\"callerid\" : 12345\n\
}";

char *add_response = "{ \n \
    \"method\" : \"add\",\n \
    \"result\" : 17,\n \
    \"callerid\" : 12346\n \
}";

char *sub_response = "{ \n \
    \"method\" : \"sub\",\n \
    \"result\" : 3.3,\n \
    \"callerid\" : 12347\n \
}";

char *mul_response = "{ \n \
    \"method\" : \"mul\",\n \
    \"result\" : 8.64,\n \
    \"callerid\" : 12348\n \
}";

char *no_response = "{ \n \
    \"method\" : \"no\",\n \
    \"result\" : \"no found\",\n \
    \"callerid\" : 12348\n \
}";

char* rpc_header_encode(char *request)
{
    if (request == NULL) {
        printf("rpc header encode failed, invalid param\n");
        return NULL;
    }
    char *header = (char *)rpc_malloc(RPC_HEADER_LENGTH);

    unsigned int crc = calc_crc32(request, strlen(request));
    unsigned short str_length = strlen(request);
    memcpy(header, &crc, 4);
    memcpy(header + 4, &str_length, 2);

    return header;
}

void *rpc_malloc(size_t size)
{
    return malloc(size);
}

void rpc_free(void *ptr)
{
    free(ptr);
}

char *rpc_process(char *request_json) 
{
    if (request_json == NULL) {
        printf("rpc_process failed, invalid param\n");
        return NULL;
    }
    cJSON *root = cJSON_Parse(request_json);
    if (root == NULL) {
        printf("connot get json root\n");
        return NULL;
    }

    cJSON *method = cJSON_GetObjectItem(root, "method");
    if (strcmp(method->valuestring, "sayhello") == 0) {
        printf("get method sayhello\n");
        return sayhello_response;
    } else if (strcmp(method->valuestring, "add") == 0) {
        printf("get method add\n");
        return add_response;
    } else if (strcmp(method->valuestring, "sub") == 0) {
        printf("get method sub\n");
        return sub_response;
    } else if (strcmp(method->valuestring, "mul") == 0) {
        return mul_response;
    }
    printf("cannot get method\n");
    return no_response;
}

const char* rpc_caller_name()
{
    void *return_address = __builtin_return_address(1);
    Dl_info info;
    if (0 != dladdr(return_address, &info)) {
        return info.dli_sname;
    }
    return NULL;
    
}

char* rpc_request_json_encode(int numargs, ...)
{
    const char *func_name = rpc_caller_name();
    if (func_name == NULL) {
        printf("func_name == NULL\n");
        return NULL;
    }
    
    cJSON *root = cJSON_CreateObject();
    
    cJSON_AddStringToObject(root, "method", func_name);
    cJSON *param = cJSON_CreateObject();
    
    return NULL;
} 

char* rpc_response_json_decode(char *response)
{

}        

char *rpc_read_register_config(char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }
    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char *buffer = rpc_malloc(file_size + 1);
    if (buffer == NULL) {
        perror("rpc_malloc");
        close(fd);
        return NULL;
    }

    int bytes_read = read(fd, buffer, file_size);
    buffer[bytes_read] = '\0';
    close(fd);

    return buffer;
}

char *rpc_server_ip = NULL;
short rpc_server_port;

struct rpc_func *rpc_caller_table = NULL;

int rpc_decode_register_json(char *json)
{
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        printf("decode register json failed\n");
        return -1;
    }

    int ret = 0;
    cJSON *cjson_remote = cJSON_GetObjectItem(root, "remote");
    if (cjson_remote && cJSON_IsString(cjson_remote)) {
        if (rpc_server_ip) {
            free(rpc_server_ip);
            rpc_server_ip = NULL;
        }

        const char *remote_str = cJSON_GetStringValue(cjson_remote);
        rpc_server_ip = remote_str;
        
    } else {
        printf("Missing or invalid 'remote' field\n");
        ret = -3;
        goto out;
    }

    cJSON *cjson_port = cJSON_GetObjectItem(root, "port");
    if (cjson_port && cJSON_IsNumber(cjson_port)) {
        rpc_server_port = cjson_port->valueint;
    } else {
        printf("Missing or invalid 'port' field\n");
        ret = -4;
        goto out;
    }

    printf("remote ---> %s:%d\n", rpc_server_ip, rpc_server_port);

    cJSON *cjson_config = cJSON_GetObjectItem(root, "config");
    int config_size = cJSON_GetArraySize(cjson_config);

    int i = 0;
    cJSON *iter = NULL;
    for (i = 0; i < config_size; ++i) {
        struct rpc_func *func = (rpc_func_t*)rpc_malloc(sizeof(rpc_func_t));
        assert(func != NULL);
        memset(func, 0, sizeof(rpc_func_t));
        iter = cJSON_GetArrayItem(cjson_config, i);
        cJSON *method = cJSON_GetObjectItem(iter, "method");
        func->method = method->valuestring;

        //todo
    }

out:
    cJSON_Delete(root);

    return ret;

}