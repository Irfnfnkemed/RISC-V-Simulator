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

public:

    //设置操作名和操作数(instruction_为未处理的指令编号)，对ALU指令进行操作，并输出结果
    int execute(int instruction, int operand_one, int operand_two);

};

#endif //RISC_V_SIMULATOR_ALU_H
