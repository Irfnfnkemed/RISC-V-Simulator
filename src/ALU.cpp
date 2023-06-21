#include "../head_file/ALU.h"

void ALU::set_ALU(int instruction_, int operand_one_, int operand_two_) {
    instruction = instruction_;
    operand_one = operand_one_;
    operand_two = operand_two_;
}

int ALU::execute() {
    int shift;
    switch (instruction) {
        case AND:
        case ANDI:
            output = operand_one & operand_two;
            break;
        case OR:
        case ORI:
            output = operand_one | operand_two;
            break;
        case XOR:
        case XORI:
            output = operand_one ^ operand_two;
            break;
        case ADD:
        case ADDI:
            output = operand_one + operand_two;
            break;
        case SUB:
            output = operand_one - operand_two;
            break;
        case SRL:
        case SRLI:
            shift = operand_two & 31;
            output = operand_one >> shift;
            break;
        case SRA:
        case SRAI:
            shift = operand_two & 31;
            output = unsigned(operand_one) >> shift;
            break;
        case SLL:
        case SLLI:
            shift = operand_two & 31;
            output = operand_one >> shift;
            break;
        case SLT:
        case SLTI:
            output = (operand_one < operand_two);
            break;
        case SLTU:
        case SLTIU:
            output = (unsigned(operand_one) < unsigned(operand_two));
    }
    return output;
}