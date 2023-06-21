#ifndef RISC_V_SIMULATOR_TOOL_H
#define RISC_V_SIMULATOR_TOOL_H

#include <cstring>

//将有digit位的16进制数转换为10进制数
int hex_to_dec(const char source[], int digit);

//将10进制数转换为digit位的2进制数
void dec_to_bin(bool target[], int source, int digit);

//将有digit位的2进制数转换为10进制数
int bin_to_dec(const bool source[], int digit);

//将有digit位(二进制下)的10进制数符号扩展为32位
int sign_extend(int source, int digit);


#endif //RISC_V_SIMULATOR_TOOL_H
