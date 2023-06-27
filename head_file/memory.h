#ifndef RISC_V_SIMULATOR_MEMORY_H
#define RISC_V_SIMULATOR_MEMORY_H

#include <cstring>
#include <iostream>
#include "tool.h"


class memory {
private:

    int memory_unit[16384];//用10进制存储16进制下的内存内容

public:

    //读入16进制数下的内存并存储
    void init();

    //读取addr处(字节寻址)byte_number个字节的内存
    int load_memory(int addr, int byte_number);

    //将source的byte_number个字节写入addr处(字节寻址)的内存
    void store_memory(int addr, int source, int byte_number);

};


#endif //RISC_V_SIMULATOR_MEMORY_H
