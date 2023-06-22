#include "../head_file/ALU.h"

int ALU::execute(int instruction, int operand_one, int operand_two) {
    int shift, output;
    switch (instruction - OFFSET) {
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