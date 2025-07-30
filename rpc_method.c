#include "rpc_method.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* rpc_sayhello(char* msg, int length)
{
	char *ret = msg;
	for (int i = 0; i < length/2; i++) {
		char tmp = msg[i];
		msg[i] = msg[length - i - 1];
		msg[length - i - 1] = tmp;
	}
	
	return ret;
}

char* rpc_response_json_encode_sayhello(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_msg = cJSON_GetObjectItem(params, "msg");
	cJSON *cjson_length = cJSON_GetObjectItem(params, "length");
	char* ret = rpc_sayhello(cjson_msg->valuestring, cjson_length->valueint);
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
	int ret = -1;
	ret = a + b;
	return ret;
}

char* rpc_response_json_encode_add(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_a = cJSON_GetObjectItem(params, "a");
	cJSON *cjson_b = cJSON_GetObjectItem(params, "b");
	int ret = rpc_add(cjson_a->valueint, cjson_b->valueint);
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
	int ret = atoi(result);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

double rpc_sub(double a, double b)
{
	double ret = -1;
	ret = a - b;
	return ret;
}

char* rpc_response_json_encode_sub(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_a = cJSON_GetObjectItem(params, "a");
	cJSON *cjson_b = cJSON_GetObjectItem(params, "b");
	double ret = rpc_sub(cjson_a->valueint, cjson_b->valueint);
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
	double ret = strtod(result, NULL);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

double rpc_mul(double a, double b)
{
	double ret = -1;
	ret = a * b;
	return ret;
}

char* rpc_response_json_encode_mul(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_a = cJSON_GetObjectItem(params, "a");
	cJSON *cjson_b = cJSON_GetObjectItem(params, "b");
	double ret = rpc_mul(cjson_a->valueint, cjson_b->valueint);
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
	double ret = strtod(result, NULL);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

int rpc_sum(int a, int b, int c)
{
	int ret = -1;
	ret = a + b + c;
	return ret;
}

char* rpc_response_json_encode_sum(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_a = cJSON_GetObjectItem(params, "a");
	cJSON *cjson_b = cJSON_GetObjectItem(params, "b");
	cJSON *cjson_c = cJSON_GetObjectItem(params, "c");
	int ret = rpc_sum(cjson_a->valueint, cjson_b->valueint, cjson_c->valueint);
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
	int ret = atoi(result);
	rpc_free(result);
	rpc_free(response);
	rpc_free(request);
	return ret;
}

char* rpc_echo(char* msg, int length)
{
	char *ret = msg;
	for (int i = 0; i < length; i++) {
		if (msg[i] >= 'a' && msg[i] <= 'z') {
			ret[i] = msg[i] - 'a' + 'A';
		} else if (msg[i] >= 'A' && msg[i] <= 'Z') {
			ret[i] = msg[i] - 'A' + 'a';
		} else {
			ret[i] = msg[i];
		}
	}
	return ret;
}

char* rpc_response_json_encode_echo(cJSON *params, rpc_task_t *task)
{
	cJSON *cjson_msg = cJSON_GetObjectItem(params, "msg");
	cJSON *cjson_length = cJSON_GetObjectItem(params, "length");
	char* ret = rpc_echo(cjson_msg->valuestring, cjson_length->valueint);
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

