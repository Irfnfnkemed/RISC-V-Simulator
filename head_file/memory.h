#ifndef RISC_V_SIMULATOR_MEMORY_H
#define RISC_V_SIMULATOR_MEMORY_H

#include <cstring>
#include <iostream>


class memory {
private:

    char memory_unit[2048][8];//存储16进制下的内存内容

public:

    void read();//读入16进制数下的内存并存储

    void load_memory(char target[], int addr, int byte_number);//读取addr处byte_number个字节的内存

};


#endif //RISC_V_SIMULATOR_MEMORY_H
