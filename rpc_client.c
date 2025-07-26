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



int main(int argc, char **argv)
{
    const char *ip = "192.168.176.128";
    unsigned short port = 9096;

    int connfd = connect_tcp_server(ip, port);
    
    char *request = "{ \n \
        \"method\" : \"sayhello\",\n \
        \"param\" : {\n \
            \"msg\" : \"zrpc nb\",\n \
            \"length\" : 7\n \
        },\n \
        \"callerid\" : 12345\n \
    }";

    char *header = rpc_header_encode(request);

    int sent = send(connfd, header, RPC_HEADER_LENGTH, 0);

    sent = send(connfd, request, strlen(request), 0);

    free(header);

    while (1) {
		char recv_header[RPC_HEADER_LENGTH] = {0};
		int ret = recv(connfd, recv_header, RPC_HEADER_LENGTH, 0);
		if (ret < 0) {
			break;
		} else if (ret == 0) {
			printf("server close\n");
			break;
		}

		unsigned int crc32 = *(unsigned int *)recv_header;
		unsigned short length = *(unsigned short *)(recv_header + 4);

		char *payload = (char *)malloc((length + 1) * sizeof(char));
		if (payload == NULL) {
			continue;
		}
		memset(payload, 0, (length + 1) * sizeof(char));
		ret = recv(connfd, payload, length, 0);
		assert(ret == length);
		if (crc32 != calc_crc32(payload, length)) {
		    printf("crc32 check failed\n");
		    free(payload);
		    continue;
		}

        printf("response payload: %s\n", payload);
		free(payload);
    }

    close(connfd);

    return 0;
}