#ifndef RISC_V_SIMULATOR_TOOL_H
#define RISC_V_SIMULATOR_TOOL_H

//将有digit位的16进制数转换为10进制数
unsigned int hex_to_dec(const char source[], int digit);

//将有digit位的16进制数转换为2进制数
void hex_to_bin(char target[], const char source[], int digit);

#endif //RISC_V_SIMULATOR_TOOL_H
