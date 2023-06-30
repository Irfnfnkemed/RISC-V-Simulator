#ifndef RISC_V_SIMULATOR_MEMORY_H
#define RISC_V_SIMULATOR_MEMORY_H

#include <cstring>
#include <iostream>
#include "tool.h"

const int size = (1 << 22);

class memory {
private:

    u_int8_t memory_unit[size];//用10进制存储16进制下的内存内容，以字节为单位

public:

    //读入16进制数下的内存并存储
    void init();

    //读取addr处(字节寻址)byte_number个字节的内存，sign_extended为真表符号扩展，反之为零扩展
    int load_memory(int addr, int byte_number, bool sign_extended = true);

    //将source的byte_number个字节写入addr处(字节寻址)的内存
    void store_memory(int addr, int source, int byte_number);

};


#endif //RISC_V_SIMULATOR_MEMORY_H
