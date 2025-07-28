#include "nty_coroutine.h"
#include "crc32.h"
#include "rpc.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>



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

int rpc_client_session(char *request_json)
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
        return -1;
    } else if (ret == 0) {
        printf("server close\n");
        return -1;
    }

    unsigned int crc32 = *(unsigned int *)recv_header;
    unsigned short length = *(unsigned short *)(recv_header + 4);

    char *payload = (char *)rpc_malloc((length + 1) * sizeof(char));
    if (payload == NULL) {
        printf("payload malloc failed\n");
        return -1;
    }
    memset(payload, 0, (length + 1) * sizeof(char));
    ret = recv(connfd, payload, length, 0);
    assert(ret == length);
    if (crc32 != calc_crc32(payload, length)) {
        printf("crc32 check failed\n");
        rpc_free(payload);
        return -1;
    }

    printf("response payload: %s\n", payload);
    rpc_free(payload);

    close(connfd);
}

char *sayhello(char *msg, int length)
{
    char *request = rpc_request_json_encode(2, msg, length);    /*generate json*/
    //char *response = rpc_client_session(request);               /*send request and recv response */
    //char *result = rpc_response_json_decode(response);          /*decode response and get result*/
}

int add(int a, int b)
{
    char *request = rpc_request_json_encode(2, a, b);
    char *response = rpc_client_session(request);
    char *result = rpc_response_json_decode(response);
}

float sub(float a, float b)
{
    char *request = rpc_request_json_encode(2, a, b);
    char *response = rpc_client_session(request);
    char *result = rpc_response_json_decode(response);
}

double mul(double a, double b)
{
    char *request = rpc_request_json_encode(2, a, b);
    char *response = rpc_client_session(request);
    char *result = rpc_response_json_decode(response);
}

int main(int argc, char **argv)
{
    sayhello(NULL, 1);
    // add(1,1);
    // sub(1,1);
    // mul(1,1);

    return 0;
}