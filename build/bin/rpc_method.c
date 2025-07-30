#include "rpc_method.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* rpc_sayhello(char* msg, int length)
{
	/*to do..*/
	return ret;
}

char* rpc_response_json_encode_sayhello(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_msg = cJSON_GetObjectItem(params, "msg");
	cJSON *cjson_length = cJSON_GetObjectItem(params, "length");
char *ret = rpc_sayhello(cjson_msg->valuestring, cjson_length->valuestring);
cJSON *root = cJSON_CreateObject();
cJSON_AddStringToObject(root, "method", task->method);
cJSON_AddStringToObject(root, "result", ret);
cJSON_AddNumberToObject(root, "callerid", task->callerid);
char *response = cJSON_Print(root);
cJSON_Delete(root);
return response;
}

char* sayhello(char* msg, int length)
{
	char *request = rpc_request_json_encode(2, msg, length);
	char *response = rpc_client_session(request);
	char *result = rpc_response_json_decode(response);
	char *ret = strdup(result);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

int rpc_add(int a, int b)
{
	/*to do..*/
	return ret;
}

int rpc_response_json_encode_add(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_a = cJSON_GetObjectItem(params, "a");
	cJSON *cjson_b = cJSON_GetObjectItem(params, "b");
char *ret = rpc_sayhello(cjson_a->valuestring, cjson_b->valuestring);
cJSON *root = cJSON_CreateObject();
cJSON_AddStringToObject(root, "method", task->method);
cJSON_AddNumberToObject(root, "result", ret);
cJSON_AddNumberToObject(root, "callerid", task->callerid);
char *response = cJSON_Print(root);
cJSON_Delete(root);
return response;
}

int add(int a, int b)
{
	char *request = rpc_request_json_encode(2, a, b);
	char *response = rpc_client_session(request);
	char *result = rpc_response_json_decode(response);
	char *ret = strdup(result);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

double rpc_sub(double a, double b)
{
	/*to do..*/
	return ret;
}

double rpc_response_json_encode_sub(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_a = cJSON_GetObjectItem(params, "a");
	cJSON *cjson_b = cJSON_GetObjectItem(params, "b");
char *ret = rpc_sayhello(cjson_a->valuestring, cjson_b->valuestring);
cJSON *root = cJSON_CreateObject();
cJSON_AddStringToObject(root, "method", task->method);
cJSON_AddNumberToObject(root, "result", ret);
cJSON_AddNumberToObject(root, "callerid", task->callerid);
char *response = cJSON_Print(root);
cJSON_Delete(root);
return response;
}

double sub(double a, double b)
{
	char *request = rpc_request_json_encode(2, a, b);
	char *response = rpc_client_session(request);
	char *result = rpc_response_json_decode(response);
	char *ret = strdup(result);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

double rpc_mul(double a, double b)
{
	/*to do..*/
	return ret;
}

double rpc_response_json_encode_mul(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_a = cJSON_GetObjectItem(params, "a");
	cJSON *cjson_b = cJSON_GetObjectItem(params, "b");
char *ret = rpc_sayhello(cjson_a->valuestring, cjson_b->valuestring);
cJSON *root = cJSON_CreateObject();
cJSON_AddStringToObject(root, "method", task->method);
cJSON_AddNumberToObject(root, "result", ret);
cJSON_AddNumberToObject(root, "callerid", task->callerid);
char *response = cJSON_Print(root);
cJSON_Delete(root);
return response;
}

double mul(double a, double b)
{
	char *request = rpc_request_json_encode(2, a, b);
	char *response = rpc_client_session(request);
	char *result = rpc_response_json_decode(response);
	char *ret = strdup(result);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

int rpc_sum(int a, int b, int c)
{
	/*to do..*/
	return ret;
}

int rpc_response_json_encode_sum(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_a = cJSON_GetObjectItem(params, "a");
	cJSON *cjson_b = cJSON_GetObjectItem(params, "b");
	cJSON *cjson_c = cJSON_GetObjectItem(params, "c");
char *ret = rpc_sayhello(cjson_a->valuestring, cjson_b->valuestring, cjson_c->valuestring);
cJSON *root = cJSON_CreateObject();
cJSON_AddStringToObject(root, "method", task->method);
cJSON_AddNumberToObject(root, "result", ret);
cJSON_AddNumberToObject(root, "callerid", task->callerid);
char *response = cJSON_Print(root);
cJSON_Delete(root);
return response;
}

int sum(int a, int b, int c)
{
	char *request = rpc_request_json_encode(3, a, b, c);
	char *response = rpc_client_session(request);
	char *result = rpc_response_json_decode(response);
	char *ret = strdup(result);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

char* rpc_echo(char* msg, int length)
{
	/*to do..*/
	return ret;
}

char* rpc_response_json_encode_echo(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_msg = cJSON_GetObjectItem(params, "msg");
	cJSON *cjson_length = cJSON_GetObjectItem(params, "length");
char *ret = rpc_sayhello(cjson_msg->valuestring, cjson_length->valuestring);
cJSON *root = cJSON_CreateObject();
cJSON_AddStringToObject(root, "method", task->method);
cJSON_AddStringToObject(root, "result", ret);
cJSON_AddNumberToObject(root, "callerid", task->callerid);
char *response = cJSON_Print(root);
cJSON_Delete(root);
return response;
}

char* echo(char* msg, int length)
{
	char *request = rpc_request_json_encode(2, msg, length);
	char *response = rpc_client_session(request);
	char *result = rpc_response_json_decode(response);
	char *ret = strdup(result);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

