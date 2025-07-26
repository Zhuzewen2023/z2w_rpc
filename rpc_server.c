#include "nty_coroutine.h"

#include <arpa/inet.h>

#define MAX_CLIENT_NUM			1000000
#define TIME_SUB_MS(tv1, tv2)  ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec) / 1000)



void rpc_handle(void *arg) {
	int fd = *(int *)arg;
	free(arg);
	int ret = 0;

	char *response = "{ \n \
	\"method\" : \"sayhello\",\n \
	\"param\" : {\n \
		\"msg\" : \"bn cprz\",\n \
		\"length\" : 7\n \
	},\n \
	\"callerid\" : 12345\n\
	}";

	while (1) {
		
		char buf[1024] = {0};
		ret = recv(fd, buf, 1024, 0);
		if (ret > 0) {
			if(fd > MAX_CLIENT_NUM) 
			printf("read from server: %.*s\n", ret, buf);

			ret = send(fd, response, strlen(response), 0);
			if (ret == -1) {
				close(fd);
				break;
			}
		} else if (ret == 0) {	
			close(fd);
			break;
		}

	}
}


void rpc_listen(void *arg) {

	unsigned short port = *(unsigned short *)arg;
	free(arg);

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
		if (cli_fd % 1000 == 999) {

			struct timeval tv_cur;
			memcpy(&tv_cur, &tv_begin, sizeof(struct timeval));
			
			gettimeofday(&tv_begin, NULL);
			int time_used = TIME_SUB_MS(tv_begin, tv_cur);
			
			printf("client fd : %d, time_used: %d\n", cli_fd, time_used);
		}
		printf("new client comming\n");

		nty_coroutine *read_co;
		int *arg = malloc(sizeof(int));
		*arg = cli_fd;
		nty_coroutine_create(&read_co, rpc_handle, arg);

	}
	
}



int main(int argc, char *argv[]) {
	nty_coroutine *co = NULL;

	int i = 0;
	unsigned short base_port = 9096;
	unsigned short *port = calloc(1, sizeof(unsigned short));
	*port = base_port;
	nty_coroutine_create(&co, rpc_listen, port); ////////no run

	nty_schedule_run(); //run

	return 0;
}



