#ifndef __CRC32_H__
#define __CRC32_H__

#ifdef __cplusplus
extern "C" {
#endif 

#include <stddef.h>

#define RPC_CRC32_TABLE_LENGTH		256

unsigned int calc_crc32(const unsigned char* data, size_t length);
int rpc_check_packet(unsigned char *payload, int length, unsigned int crc32);

#ifdef __cplusplus
}
#endif

#endif