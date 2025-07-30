#ifndef __RPC_METHOD_H__
#define __RPC_METHOD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cJSON.h"
#include "rpc.h"
#include "rpc_client.h"

char* sayhello(char* msg, int length);
char* rpc_response_json_encode_sayhello(cJSON *params, rpc_task_t *task);
char* rpc_sayhello(char* msg, int length);
int add(int a, int b);
int rpc_response_json_encode_add(cJSON *params, rpc_task_t *task);
int rpc_add(int a, int b);
double sub(double a, double b);
double rpc_response_json_encode_sub(cJSON *params, rpc_task_t *task);
double rpc_sub(double a, double b);
double mul(double a, double b);
double rpc_response_json_encode_mul(cJSON *params, rpc_task_t *task);
double rpc_mul(double a, double b);
int sum(int a, int b, int c);
int rpc_response_json_encode_sum(cJSON *params, rpc_task_t *task);
int rpc_sum(int a, int b, int c);
char* echo(char* msg, int length);
char* rpc_response_json_encode_echo(cJSON *params, rpc_task_t *task);
char* rpc_echo(char* msg, int length);
#ifdef __cplusplus
}
#endif

#endif
