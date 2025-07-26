#ifndef __RPC_H__
#define __RPC_H__

#ifdef __cplusplus
extern "C" {
#endif

#define RPC_HEADER_LENGTH   6 //CRC 4BYTES + LENGTH 2BYTES

char* rpc_header_encode(char *request);

#ifdef __cplusplus
}
#endif

#endif
