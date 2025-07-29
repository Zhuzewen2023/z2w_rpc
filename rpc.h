#ifndef __RPC_H__
#define __RPC_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>

#define RPC_HEADER_LENGTH       6   //CRC 4BYTES + LENGTH 2BYTES
#define RPC_PARAMS_MAX_COUNT    16

extern char *sayhello_request;
extern char *add_request;
extern char *sub_request;
extern char *mul_request;
extern char *sayhello_response;
extern char *add_response;
extern char *sub_response;
extern char *mul_response;
extern char *no_response;
extern char *rpc_server_ip;
extern short rpc_server_port;
extern struct rpc_func *rpc_caller_table;

typedef struct rpc_func
{
    char *method;
    char *params[RPC_PARAMS_MAX_COUNT];
    char *types[RPC_PARAMS_MAX_COUNT];
    int count;  // count is the number of params
    char *rettype;
    struct rpc_func *next;
}rpc_func_t;

char* rpc_header_encode(char *request);
void *rpc_malloc(size_t size);
void rpc_free(void *ptr);
char *rpc_process(char *request_json);
const char* rpc_caller_name();
char* rpc_request_json_encode(int numargs, ...);
char* rpc_response_json_decode(char *response);  
char *rpc_read_register_config(char *filename);
int rpc_decode_register_json(char *json);

#ifdef __cplusplus
}
#endif

#endif
