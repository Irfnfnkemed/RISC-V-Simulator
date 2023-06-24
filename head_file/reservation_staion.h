#ifndef RISC_V_SIMULATOR_RESERVATION_STAION_H
#define RISC_V_SIMULATOR_RESERVATION_STAION_H

#include "algorithm_logic_unit.h"
#include "reorder_buffer.h"
#include "tool.h"

class reservation_station {
private:

    enum INSTRUCTION {
        ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
        ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
    };

    const int OFFSET = 18;//reservation_station对应的指令编号为18至36，初始指令偏移量为18

    struct reservation_unit {
        int instruction = -1;//指令编号(减去OFFSET)，为-1时表示该位置为空
        int value_one, value_two;//两个寄存器(若存在)的值
        int depend_one = -1, depend_two = -1;//两个寄存器(若存在)对前面指令的依赖关系，存储依赖指令的tag，无依赖时默认为-1
        int immediate;//存放立即数(或者运算结果)
        int destination;//最终存放的目标寄存器编号(若存在)
        int tag;//指令在ROB中的tag
        int status = 0;//0表未计算，1表执行但未提交，2表已经提交至ROB
    };

    algorithm_logic_unit *ALU;
    reorder_buffer *Reorder_buffer;

    reservation_unit buffer[16];

public:

    void init(reorder_buffer *Reorder_buffer_);

    //添加任务，instruction_为未处理的指令编号
    void add_instruction(int instruction_, int value_one_, int value_two_,
                         int depend_one_, int depend_two_,
                         int immediate_, int destination_, int tag_);

    void execute_rising_edge();

    void execute_falling_edge();

    //监听CDB。删去标签为tag_的指令，并修改reg_对应的值。若不修改，则用-1表示
    void response_CDB(int tag_, int reg_, int data_);

};

#endif //RISC_V_SIMULATOR_RESERVATION_STAION_H
