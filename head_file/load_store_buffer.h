#ifndef RISC_V_SIMULATOR_LOAD_STORE_BUFFER_H
#define RISC_V_SIMULATOR_LOAD_STORE_BUFFER_H

#include "memory.h"
#include "algorithm_logic_unit.h"
#include "tool.h"
#include "reorder_buffer.h"

class load_store_buffer {
private:
    enum INSTRUCTION {
        LB, LH, LW, LBU, LHU, SB, SH, SW
    };
    struct load_store_unit {
        int instruction;//load_store_buffer相关的指令编号(减去OFFSET)
        int value_one, value_two;//两个相关寄存器的值
        int depend_one = -1, depend_two = -1;//两个相关寄存器对前面指令的依赖关系，存储相应的寄存器编号，无依赖时默认为-1
        int offset;//偏移量
        int destination;//存指令的这一项是无用的
        int tag;//读指令在reservation_station中对应的tag，写指令的tag值是无用的
        bool execute = false;//是否已经计算出地址
    };

    const int OFFSET = 10;//load_store_buffer对应的指令编号为10至17，初始指令偏移量为10

    queue<load_store_unit, 64> buffer;//循环队列
    int clock_time = 0;
    address_algorithm_logic_unit address_ALU;
    memory *Memory;
    reorder_buffer *Reorder_buffer;

public:

    //初始化，关联相关模块
    void init(memory *Memory_, reorder_buffer *Reorder_buffer_);

    //添加任务，instruction_为未处理的指令编号
    void push_buffer(int instruction_, int value_one_, int value_two_,
                     int depend_one_, int depend_two_,
                     int offset, int destination_, int tag_);

    void execute_rising_edge();

    void execute_falling_edge();

    //监听CDB
    void response_CDB(int reg_, int data_);
};

#endif //RISC_V_SIMULATOR_LOAD_STORE_BUFFER_H
