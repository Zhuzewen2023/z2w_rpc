#include "nty_coroutine.h"
#include "crc32.h"
#include "rpc.h"

#include <arpa/inet.h>

#define MAX_CLIENT_NUM			1000000
#define TIME_SUB_MS(tv1, tv2)  ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec) / 1000)





void rpc_handle(void *arg) {
	int fd = *(int *)arg;
	rpc_free(arg);
	int ret = 0;



	while (1) {
		char header[RPC_HEADER_LENGTH] = {0};
		int ret = recv(fd, header, RPC_HEADER_LENGTH, 0);
		if (ret < 0) {
			break;
		} else if (ret == 0) {
			printf("client close\n");
			break;
		}

		unsigned int crc32 = *(unsigned int *)header;
		unsigned short length = *(unsigned short *)(header + 4);

		char *payload = (char *)rpc_malloc((length + 1) * sizeof(char));
		if (payload == NULL) {
			continue;
		}
		memset(payload, 0, (length + 1) * sizeof(char));
		ret = recv(fd, payload, length, 0);
		assert(ret == length);

		printf("request payload: %s\n", payload);
		if (crc32 != calc_crc32(payload, length)) {
			printf("crc32 check failed\n");
			rpc_free(payload);
			break;
		}

		char *response = rpc_process(payload);

		char *response_header = rpc_header_encode(response);
		ret = send(fd, response_header, RPC_HEADER_LENGTH, 0);

		ret = send(fd, response, strlen(response), 0);
		rpc_free(response_header);
		rpc_free(payload);

	}
	close(fd);
	fd = -1;
	
}


void rpc_listen(void *arg) {

	unsigned short port = *(unsigned short *)arg;
	rpc_free(arg);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) return ;

	struct sockaddr_in local, remote;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = INADDR_ANY;
	bind(fd, (struct sockaddr*)&local, sizeof(struct sockaddr_in));

	listen(fd, 20);
	printf("listen port : %d\n", port);

	
	struct timeval tv_begin;
	gettimeofday(&tv_begin, NULL);

	while (1) {
		socklen_t len = sizeof(struct sockaddr_in);
		int cli_fd = accept(fd, (struct sockaddr*)&remote, &len);
		nty_coroutine *read_co;
		int *arg = rpc_malloc(sizeof(int));
		*arg = cli_fd;
		nty_coroutine_create(&read_co, rpc_handle, arg);

	}
	
}



int main(int argc, char *argv[]) 
{
	if (argc != 2) {
		printf("Usage: ./%s <filename>\n", argv[0]);
		return -1;
    }

    rpc_load_register(argv[1]);

	nty_coroutine *co = NULL;

	int i = 0;
	unsigned short base_port = 9096;
	unsigned short *port = calloc(1, sizeof(unsigned short));
	*port = base_port;
	nty_coroutine_create(&co, rpc_listen, port); ////////no run

	nty_schedule_run(); //run

	return 0;
}



