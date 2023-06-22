#ifndef RISC_V_SIMULATOR_LOAD_STORE_BUFFER_H
#define RISC_V_SIMULATOR_LOAD_STORE_BUFFER_H

#include "memory.h"
#include "tool.h"

class load_store_buffer {
private:
    enum INSTRUCTION {
        LB, LH, LW, LBU, LHU, SB, SH, SW
    };
    struct load_store_unit {
        int instruction;//load_store_buffer相关的指令编号(减去OFFSET)
        int addr, data;
        int tag;//读指令在reservation_station中对应的tag，写指令的tag值是无用的
    };

    const int OFFSET = 10;//load_store_buffer对应的指令编号为10至17，初始指令偏移量为10

    queue<load_store_unit, 64> buffer;//循环队列
    int clock_time = 0;
    memory *Memory;

public:

    //初始化，关联相关模块
    void init(memory *Memory_);

    //添加任务，instruction_为未处理的指令编号
    void push_buffer(int instruction_, int addr_, int data_, int tag_);

    void execute();
};

#endif //RISC_V_SIMULATOR_LOAD_STORE_BUFFER_H
