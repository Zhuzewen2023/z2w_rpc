#define _GNU_SOURCE

#include "rpc.h"
#include "crc32.h"
#include "cJSON.h"
#include "rpc_method.h"
#include "rpc_client.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <link.h>
#include <dlfcn.h>
#include <assert.h>
#include <stdarg.h>
#include <arpa/inet.h>


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
    cJSON *params = cJSON_GetObjectItem(root, "params");
    cJSON *callerid = cJSON_GetObjectItem(root, "callerid");

    rpc_func_t* func = rpc_get_caller_table();
    while (func) {
        if (0 == strcmp(func->method, method->valuestring)) {
            break;
        }
        func = func->next;
    }
    if (func == NULL) {
        printf("cannot get func\n");
        return NULL;
    }
    rpc_task_t *task = (rpc_task_t *)rpc_malloc(sizeof(rpc_task_t));
    task->method = method->valuestring;
    task->callerid = callerid->valueint;

    char *response = func->handler(params, task);
    return response;
    // if (strcmp(method->valuestring, "sayhello") == 0) {
    //     // printf("get method sayhello\n");
    //     // return sayhello_response;
    //     return rpc_response_json_encode_sayhello(params, task);
    // } else if (strcmp(method->valuestring, "add") == 0) {
    //     // printf("get method add\n");
    //     // return add_response;
    //     return rpc_response_json_encode_add(params, task);
    // } else if (strcmp(method->valuestring, "sub") == 0) {
    //     // printf("get method sub\n");
    //     // return sub_response;
    //     return rpc_response_json_encode_sub(params, task);
    // } else if (strcmp(method->valuestring, "mul") == 0) {
    //     // return mul_response;
    //     return rpc_response_json_encode_mul(params, task);
    // }
    // printf("cannot get method\n");
    // return no_response;
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
    // printf("func_name = %s\n", func_name);
    
    rpc_func_t* func = rpc_get_caller_table();
    while (func) {
        if (strcmp(func->method, func_name) == 0) {
            //printf("found func_name = %s\n", func->method);
            break;
        }
        func = func->next;
    }

    if (func == NULL) {
        printf("func == NULL\n");
        return NULL;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "method", func->method);
    cJSON *params = cJSON_CreateObject();

    va_list args;
    va_start(args, numargs);
    int i = 0;
    // printf("func->count = %d\n", func->count);
    for (i = 0; i < func->count; ++i) {
        //printf("func->types[%d] = %s\n", i, func->types[i]);
        if (0 == strcmp(func->types[i], "int")) {
            cJSON_AddNumberToObject(params, func->params[i], va_arg(args, int));
        } else if (0 == strcmp(func->types[i], "float")) {
            cJSON_AddNumberToObject(params, func->params[i], va_arg(args, double));
        } else if (0 == strcmp(func->types[i], "double")) {
            cJSON_AddNumberToObject(params, func->params[i], va_arg(args, double));
        } else if (0 == strcmp(func->types[i], "char*")) {
            cJSON_AddStringToObject(params, func->params[i], va_arg(args, char *));
        } else {
            assert(0);
        }
    }
    va_end(args);

    cJSON_AddItemToObject(root, "params", params);
    cJSON_AddNumberToObject(root, "callerid", rpc_get_caller_id());

    char *json = cJSON_Print(root);
    // cJSON_Delete(params);
    cJSON_Delete(root);
    
    return json;
}

char* rpc_request_json_decode(char *request)
{
    
}

char* rpc_response_json_decode(char *response)
{
    cJSON *root = cJSON_Parse(response);
    if (root == NULL) {
        printf("decode response json failed\n");
        return NULL;
    }
    cJSON *result = cJSON_GetObjectItem(root, "result");
    if (result == NULL) {
        printf("decode response json failed\n");
        return NULL;
    }

    char *result_str = cJSON_Print(result);
    cJSON_Delete(root);

    return result_str;
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

static rpc_func_t *rpc_caller_table = NULL;

rpc_func_t* rpc_get_caller_table()
{
    return rpc_caller_table;
}

#define RPC_METHOD_LIBSO "librpc_method.so"
void* dlh = NULL; //动态库句柄

void* rpc_method_find(char *funcname)
{
    if (dlh == NULL) {
        dlh = dlopen(RPC_METHOD_LIBSO, RTLD_LAZY); //RTLD_LAZY延迟绑定，在函数第一次被调用时才解析符号
        if (dlh == NULL) {
            printf("dlopen failed: %s\n", dlerror());
            return NULL;
        }
    }

    void *method = dlsym(dlh, funcname);
    if (method == NULL) {
        printf("dlsym failed: %s\n", dlerror());
        return NULL;
    }

    return method;
}

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

        cJSON *rettype = cJSON_GetObjectItem(iter, "rettype");
        func->rettype = rettype->valuestring;

        //printf("method ---> %s, rettype ---> %s\n", func->method, func->rettype);

        cJSON *params = cJSON_GetObjectItem(iter, "params");
        int params_size = cJSON_GetArraySize(params);
        cJSON *types = cJSON_GetObjectItem(iter, "types");
        int types_size = cJSON_GetArraySize(types);

        assert(params_size == types_size);

        int j = 0;
        for (j = 0; j < params_size; ++j) {
            cJSON *param = cJSON_GetArrayItem(params, j);
            cJSON *type = cJSON_GetArrayItem(types, j);
            func->params[j] = param->valuestring;
            func->types[j] = type->valuestring;
            //printf("param ---> %s, type ---> %s\n", func->params[j], func->types[j]);  
        }
        char method_name[64] = {0};
        snprintf(method_name, sizeof(method_name), "rpc_response_json_encode_%s", func->method);
        func->handler = rpc_method_find(method_name);
        func->count = params_size; // count is the number of params
        func->next = rpc_caller_table;
        rpc_caller_table = func;
    }
    return 0;

out:
    cJSON_Delete(root);

    return ret;

}

static int rpc_global_caller_id;
int rpc_get_caller_id()
{
    return rpc_global_caller_id++; //atomic
}

int rpc_load_register(char *filename)
{
    int ret = -1;
    char *json = rpc_read_register_config(filename);
    //printf("register json: %s\n", json);
    ret = rpc_decode_register_json(json);
    free(json);
    return ret;
}


char* rpc_client_session(char *request_json)
{
    const char *ip = "192.168.245.129";
    unsigned short port = 9096;

    int connfd = connect_tcp_server(ip, port);
    
    char *request = request_json;

    char *header = rpc_header_encode(request);

    int sent = send(connfd, header, RPC_HEADER_LENGTH, 0);

    sent = send(connfd, request, strlen(request), 0);

    rpc_free(header);

    char recv_header[RPC_HEADER_LENGTH] = {0};
    int ret = recv(connfd, recv_header, RPC_HEADER_LENGTH, 0);
    if (ret < 0) {
        printf("recv error\n");
        return NULL;
    } else if (ret == 0) {
        printf("server close\n");
        return NULL;
    }

    unsigned int crc32 = *(unsigned int *)recv_header;
    unsigned short length = *(unsigned short *)(recv_header + 4);

    char *payload = (char *)rpc_malloc((length + 1) * sizeof(char));
    if (payload == NULL) {
        printf("payload malloc failed\n");
        return NULL;
    }
    memset(payload, 0, (length + 1) * sizeof(char));
    ret = recv(connfd, payload, length, 0);
    assert(ret == length);
    if (crc32 != calc_crc32(payload, length)) {
        printf("crc32 check failed\n");
        rpc_free(payload);
        return NULL;
    }

    //printf("response payload: %s\n", payload);
    //rpc_free(payload);

    close(connfd);
    return payload;
}

int connect_tcp_server(const char *ip, unsigned short port)
{
    int connfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(connfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in))){
        perror("connect");
        return -1;
    }

    return connfd;
}