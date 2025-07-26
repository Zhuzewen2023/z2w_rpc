#include "crc32.h"

static unsigned int crc32_table[RPC_CRC32_TABLE_LENGTH] = {0};


/*  
*   这段代码中使用了GCC的扩展属性`__attribute__`，
*   具体是`__attribute__((constructor(1000)))`。
*   这个属性用于指定一个函数在`main`函数执行之前自动执行。
*   括号中的数字表示优先级（priority），数字越小，优先级越高，执行越早。
*   这里设置为1000，表示这个初始化函数的优先级为1000。
*/
static void __attribute__((constructor(1000))) init_crc32_table() 
{    
	unsigned int crc;    
	for (int i = 0; i < RPC_CRC32_TABLE_LENGTH; ++i) {        
		crc = i;        
		for (int j = 0; j < 8; ++j) {            
			if (crc & 1)                
				crc = (crc >> 1) ^ 0xEDB88320;            
			else                
				crc >>= 1;        
		}        
		crc32_table[i] = crc;    
	}
}

unsigned int calc_crc32(const unsigned char* data, size_t length) 
{    
	unsigned int crc = 0xFFFFFFFF;   
	
	for (size_t i = 0; i < length; ++i) {        
		crc = (crc >> 8) ^ crc32_table[(crc ^ data[i]) & 0xFF];    
	}    
	return ~crc;
}

int rpc_check_packet(unsigned char *payload, int length, unsigned int crc32)
{
    unsigned int checksum = calc_crc32(payload, length);
    if (checksum != crc32) {
        return -1;
    }
    return 0;
}