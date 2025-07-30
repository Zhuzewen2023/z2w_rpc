#include "nty_coroutine.h"
#include "crc32.h"
#include "rpc.h"
#include "rpc_method.h"
#include "rpc_client.h"

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





int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./%s <filename>\n", argv[0]);
        return -1;
    }

    rpc_load_register(argv[1]);
    printf("%s\n", sayhello("z2w rpc nb", 10));
    printf("%d\n", add(100,99));
    printf("%f\n", sub(20,3));
    printf("%f\n", mul(245,377));
    printf("%d\n", sum(111,222,333));

    return 0;
}