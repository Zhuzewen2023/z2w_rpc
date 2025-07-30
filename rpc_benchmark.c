#include "nty_coroutine.h"
#include "crc32.h"
#include "rpc.h"
#include "rpc_method.h"
#include "rpc_client.h"
#include "rpc_generator.h"

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

double get_time_diff_ms(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000.0;
}

static int func_iterations;

static void test_sayhello()
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < func_iterations; i++) {
        sayhello("z2w rpc nb", 10);
    }

    gettimeofday(&end, NULL);
    printf("sayhello: %lf avg ms\n", get_time_diff_ms(start, end)/func_iterations);
    printf("qps sayhello: %lf\n", func_iterations/(get_time_diff_ms(start, end) / 1000));
}

static void test_add()
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < func_iterations; i++) {
        add(100,99);
    }

    gettimeofday(&end, NULL);
    printf("add: %lf avg ms\n", get_time_diff_ms(start, end)/func_iterations);
    printf("qps add: %lf\n", func_iterations/(get_time_diff_ms(start, end) / 1000));
}

static void test_sub()
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < func_iterations; i++) {
        sub(20,3);
    }

    gettimeofday(&end, NULL);
    printf("sub: %lf avg ms\n", get_time_diff_ms(start, end)/func_iterations);
    printf("qps sub: %lf\n", func_iterations/(get_time_diff_ms(start, end) / 1000));
}

static void test_mul()
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < func_iterations; i++) {
        mul(245,377);
    }

    gettimeofday(&end, NULL);
    printf("mul: %lf avg ms\n", get_time_diff_ms(start, end)/func_iterations);
    printf("qps mul: %lf\n", func_iterations/(get_time_diff_ms(start, end) / 1000));
}

static void test_sum()
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < func_iterations; i++) {
        sum(111,222,333);
    }

    gettimeofday(&end, NULL);
    printf("sum: %lf avg ms\n", get_time_diff_ms(start, end)/func_iterations);
    printf("qps sum: %lf\n", func_iterations/(get_time_diff_ms(start, end) / 1000));
}

static void test_echo()
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < func_iterations; i++) {
        echo("z2w niubi NIUBI", 15);
    }

    gettimeofday(&end, NULL);
    printf("echo: %lf avg ms\n", get_time_diff_ms(start, end)/func_iterations);
    printf("qps echo: %lf\n", func_iterations/(get_time_diff_ms(start, end) / 1000));
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: ./%s <filename> <func_iterations>\n", argv[0]);
        return -1;
    }

    func_iterations = atoi(argv[2]);
    if (func_iterations <= 0) {
        printf("func_iterations must be greater than 0\n");
        return -2;
    }

    rpc_load_register(argv[1]);
    test_sayhello();
    test_add();
    test_sub();
    test_mul();
    test_sum();
    test_echo();
    //printf("%s\n", sayhello("z2w rpc nb", 10));
    //printf("%d\n", add(100,99));
    //printf("%f\n", sub(20,3));
    //printf("%f\n", mul(245,377));
    //printf("%d\n", sum(111,222,333));
    //printf("%s\n", echo("z2w niubi NIUBI", 15));

    return 0;
}