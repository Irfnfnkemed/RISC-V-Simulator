#ifndef RISC_V_SIMULATOR_ALU_H
#define RISC_V_SIMULATOR_ALU_H

#include "../head_file/tool.h"

class ALU {
private:
    enum INSTRUCTION {
        ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
        ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
    };

    const int OFFSET = 18;//ALU对应的指令编号为18至36，初始指令偏移量为18

    int instruction;//指令
    int operand_one, operand_two;//两个操作数，若有立即数，放在operand_two
    int output;//结果

public:

    //设置操作名和操作数
    void set_ALU(int instruction_, int operand_one_, int operand_two_);

    //对当前的ALU指令进行操作，并输出结果
    int execute();

};

#endif //RISC_V_SIMULATOR_ALU_H
