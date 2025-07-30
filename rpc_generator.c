#include "rpc_generator.h"
#include "cJSON.h"
#include "rpc.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void 
rpc_generate_h_method(FILE* method_h_fp, char *method_str, char *type_str, 
    char** param_str_arr, char** type_str_arr, int param_num)
{
    if (method_h_fp == NULL || method_str == NULL || type_str == NULL || 
        param_str_arr == NULL || type_str_arr == NULL) {
        return;
    }

    fprintf(method_h_fp, "%s %s(", type_str, method_str);
    for (int i = 0; i < param_num; i++) {
        fprintf(method_h_fp, "%s %s", type_str_arr[i], param_str_arr[i]);
        if (i != param_num - 1) {
            fprintf(method_h_fp, ", ");
        }
    }
    fprintf(method_h_fp, ");\n");

    fprintf(method_h_fp, "%s rpc_response_json_encode_%s(cJSON *params, rpc_task_t *task);\n",type_str, method_str);

    fprintf(method_h_fp, "%s rpc_%s(", type_str, method_str);
    for (int i = 0; i < param_num; i++) {
        fprintf(method_h_fp, "%s %s", type_str_arr[i], param_str_arr[i]);
        if (i != param_num - 1) {
            fprintf(method_h_fp, ", ");
        }
    }
    fprintf(method_h_fp, ");\n");
}

static void
rpc_generate_c_method(FILE* method_c_fp, char *method_str, char *type_str,
    char** param_str_arr, char** type_str_arr, int param_num)
{
    if (method_c_fp == NULL || method_str == NULL || type_str == NULL || 
        param_str_arr == NULL || type_str_arr == NULL) {
        return;
    }

    fprintf(method_c_fp, "%s rpc_%s(", type_str, method_str);
    for (int i = 0; i < param_num; i++) {
        fprintf(method_c_fp, "%s %s", type_str_arr[i], param_str_arr[i]);
        if (i != param_num - 1) {
            fprintf(method_c_fp, ", ");
        }
    }
    fprintf(method_c_fp, ")\n{\n");
    fprintf(method_c_fp, "\t/*to do..*/\n");
    fprintf(method_c_fp, "\treturn ret;\n");
    fprintf(method_c_fp, "}\n\n");

    fprintf(method_c_fp, "%s rpc_response_json_encode_%s(", type_str, method_str);
    fprintf(method_c_fp, "cJSON *params, rpc_task_t *task)\n{\n");
    for (int i = 0; i < param_num; i++) {
        fprintf(method_c_fp, "\tcJSON *cjson_%s = cJSON_GetObjectItem(params, \"%s\");\n", 
            param_str_arr[i], param_str_arr[i]);
    }
    fprintf(method_c_fp, "char *ret = rpc_sayhello(");
    for (int i = 0; i < param_num; i++) {
        fprintf(method_c_fp, "cjson_%s->valuestring", param_str_arr[i]);
        if (i != param_num - 1) {
            fprintf(method_c_fp, ", ");
        }
    }
    fprintf(method_c_fp, ");\n");

    fprintf(method_c_fp, "cJSON *root = cJSON_CreateObject();\n");
    fprintf(method_c_fp, "cJSON_AddStringToObject(root, \"method\", task->method);\n");
    if (0 == strcmp(type_str, "char*") || 0 == strcmp(type_str, "char *")) {
        fprintf(method_c_fp, "cJSON_AddStringToObject(root, \"result\", ret);\n");
    } else {
        fprintf(method_c_fp, "cJSON_AddNumberToObject(root, \"result\", ret);\n");
    }
    fprintf(method_c_fp, "cJSON_AddNumberToObject(root, \"callerid\", task->callerid);\n");
    fprintf(method_c_fp, "char *response = cJSON_Print(root);\n");
    fprintf(method_c_fp, "cJSON_Delete(root);\n");
    fprintf(method_c_fp, "return response;\n");
    fprintf(method_c_fp, "}\n\n");

    fprintf(method_c_fp, "%s %s(", type_str, method_str);
    for (int i = 0; i < param_num; i++) {
        fprintf(method_c_fp, "%s %s", type_str_arr[i], param_str_arr[i]);
        if (i != param_num - 1) {
            fprintf(method_c_fp, ", ");
        }
    }
    fprintf(method_c_fp, ")\n{\n");
    fprintf(method_c_fp, "\tchar *request = rpc_request_json_encode(%d, ", param_num);
    for (int i = 0; i < param_num; i++) {
        fprintf(method_c_fp, "%s", param_str_arr[i]);
        if (i != param_num - 1) {
            fprintf(method_c_fp, ", ");
        }
    }
    fprintf(method_c_fp, ");\n");
    fprintf(method_c_fp, "\tchar *response = rpc_client_session(request);\n");
    fprintf(method_c_fp, "\tchar *result = rpc_response_json_decode(response);\n");
    fprintf(method_c_fp, "\tchar *ret = strdup(result);\n");
    fprintf(method_c_fp, "\trpc_free(result);\n");
    fprintf(method_c_fp, "\trpc_free(response);\n");
    fprintf(method_c_fp, "\trpc_free(request);\n");
    fprintf(method_c_fp, "\treturn ret;\n");
    fprintf(method_c_fp, "}\n\n");
}


int 
rpc_generate_method_code(const char *register_json_path)
{
    int ret = -1;
    int fd = open(register_json_path, O_RDONLY);
    if (fd < 0) {
        perror("open register json file failed");
        return -1;
    }

    int file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char *json_buf = malloc(file_size + 1);
    if (json_buf == NULL) {
        perror("malloc json_buf failed");
        goto out;
    }
    
    if (read(fd, json_buf, file_size) != file_size) {
        perror("read register json file failed");
        goto out;
    }

    /*create file rpc_method.c, rpc_method.h */
    FILE* method_c_fp = NULL;
    FILE* method_h_fp = NULL;
    method_c_fp = fopen("rpc_method.c", "w");
    method_h_fp = fopen("rpc_method.h", "w");

    if (method_c_fp == NULL || method_h_fp == NULL) {
        perror("open rpc_method.c or rpc_method.h failed");
        goto out;
    }

    fprintf(method_h_fp, "#ifndef __RPC_METHOD_H__\n");
    fprintf(method_h_fp, "#define __RPC_METHOD_H__\n\n");
    
    fprintf(method_h_fp, "#ifdef __cplusplus\n");
    fprintf(method_h_fp, "extern \"C\" {\n");
    fprintf(method_h_fp, "#endif\n\n");

    fprintf(method_h_fp, "#include \"cJSON.h\"\n");
    fprintf(method_h_fp, "#include \"rpc.h\"\n");
    fprintf(method_h_fp, "#include \"rpc_client.h\"\n\n");

    fprintf(method_c_fp, "#include \"rpc_method.h\"\n\n");
    fprintf(method_c_fp, "#include <stdio.h>\n");
    fprintf(method_c_fp, "#include <stdlib.h>\n");
    fprintf(method_c_fp, "#include <string.h>\n\n");

    cJSON *root = cJSON_Parse(json_buf);
    if (root == NULL) {
        printf("parse register json file failed\n");
        goto out;
    }

    cJSON *config_arr = cJSON_GetObjectItem(root, "config");
    if (config_arr == NULL) {
        printf("get config items failed\n");
        goto out;
    }
    int config_arr_size = cJSON_GetArraySize(config_arr);
    for (int i = 0; i < config_arr_size; i++) {
        cJSON *config = cJSON_GetArrayItem(config_arr, i);
        if (config == NULL) {
            printf("get config item failed\n");
            goto out;
        }
        cJSON *method = cJSON_GetObjectItem(config, "method");
        if (method == NULL) {
            printf("get method name failed\n");
            goto out;
        }
        cJSON *rettype = cJSON_GetObjectItem(config, "rettype");
        if (rettype == NULL) {
            printf("get rettype failed\n");
            goto out;
        }
        cJSON *param_arr = cJSON_GetObjectItem(config, "params");
        cJSON *types_arr = cJSON_GetObjectItem(config, "types");
        if (param_arr == NULL || types_arr == NULL) {
            printf("get params or types failed\n");
            goto out;
        }
        int param_arr_size = cJSON_GetArraySize(param_arr);
        int types_arr_size = cJSON_GetArraySize(types_arr);
        if (param_arr_size != types_arr_size) {
            printf("params and types size not equal\n");
            goto out;
        }

        char **param_str_arr = rpc_malloc(param_arr_size);
        char **type_str_arr = rpc_malloc(types_arr_size);

        if (param_str_arr == NULL || type_str_arr == NULL) {
            printf("malloc param_str_arr or type_str_arr failed\n");
            rpc_free(param_str_arr);
            rpc_free(type_str_arr);
            goto out;
        }

        for (int j = 0; j < param_arr_size; j++) {
            cJSON *param = cJSON_GetArrayItem(param_arr, j);
            cJSON *type = cJSON_GetArrayItem(types_arr, j);
            if (param == NULL || type == NULL) {
                printf("get param or type failed\n");
                rpc_free(param_str_arr);
                rpc_free(type_str_arr);
                goto out;
            }
            param_str_arr[j] = cJSON_GetStringValue(param);
            type_str_arr[j] = cJSON_GetStringValue(type);
        }

        char *method_str = cJSON_GetStringValue(method);
        char *type_str = cJSON_GetStringValue(rettype);

        rpc_generate_h_method(method_h_fp, method_str, type_str, param_str_arr, type_str_arr, param_arr_size);
        rpc_generate_c_method(method_c_fp, method_str, type_str, param_str_arr, type_str_arr, param_arr_size);
        
        
        rpc_free(param_str_arr);
        rpc_free(type_str_arr);

    }

    
    fprintf(method_h_fp, "#ifdef __cplusplus\n");
    fprintf(method_h_fp, "}\n");
    fprintf(method_h_fp, "#endif\n\n");
    
    fprintf(method_h_fp, "#endif\n");


out:
    if (fd >= 0) {
        close(fd);
    }
    if (method_c_fp == NULL) {
        fclose(method_c_fp);
    }
    if (method_h_fp == NULL) {
        fclose(method_h_fp);
    }
    if (json_buf != NULL) {
        free(json_buf);
    }
    if (root != NULL) {
        cJSON_Delete(root);
    }
    return ret;
}