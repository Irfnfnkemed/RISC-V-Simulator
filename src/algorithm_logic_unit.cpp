#include "algorithm_logic_unit.h"

bool base_ALU::is_free() { return free; }

void base_ALU::flush() { free = 2; }

int logic_ALU::execute(int instruction, int operand_one, int operand_two) {
    --free;
    switch (instruction) {
        case AND:
        case ANDI:
            return operand_one & operand_two;
        case OR:
        case ORI:
            return operand_one | operand_two;
        case XOR:
        case XORI:
            return operand_one ^ operand_two;
    }
}

int add_ALU::execute(int instruction, int operand_one, int operand_two) {
    --free;
    switch (instruction) {
        case ADD:
        case ADDI:
            return operand_one + operand_two;
        case SUB:
            return operand_one - operand_two;
        case JALR:
            return (operand_one + operand_two) & (-2);
    }
}

int shift_ALU::execute(int instruction, int operand_one, int operand_two) {
    --free;
    switch (instruction) {
        case SRL:
        case SRLI:
            return operand_one >> (operand_two & 31);
        case SRA:
        case SRAI:
            return (unsigned(operand_one)) >> (operand_two & 31);
        case SLL:
        case SLLI:
            return operand_one >> (operand_two & 31);
    }
}

int compare_ALU::execute(int instruction, int operand_one, int operand_two) {
    --free;
    switch (instruction) {
        case SLT:
        case SLTI:
        case BLT:
            return (operand_one < operand_two);
        case SLTU:
        case SLTIU:
        case BLTU:
            return (unsigned(operand_one) < unsigned(operand_two));
        case BEQ:
            return (operand_one == operand_two);
        case BNE:
            return (operand_one != operand_two);
        case BGE:
            return (operand_one >= operand_two);
        case BGEU:
            return (unsigned(operand_one) >= unsigned(operand_two));
    }
}

int address_ALU::execute(int addr, int offset, int useless) {
    --free;
    return addr + offset;
}

int all_ALU::get_ALU_unit(int instruction) {
    switch (instruction) {
        case AND:
        case ANDI:
        case OR:
        case ORI:
        case XOR:
        case XORI:
            return logic;
        case ADD:
        case ADDI:
        case JALR:
        case SUB:
            return add;
        case SRL:
        case SRLI:
        case SRA:
        case SRAI:
        case SLL:
        case SLLI:
            return shift;
        case SLT:
        case SLTI:
        case SLTU:
        case SLTIU:
        case BEQ:
        case BNE:
        case BLT:
        case BGE:
        case BLTU:
        case BGEU:
            return compare;
    }
}

bool all_ALU::is_free(int instruction) {
    switch (get_ALU_unit(instruction)) {
        case logic:
            return Logic_ALU.is_free();
        case add:
            return Add_ALU.is_free();
        case shift:
            return Shift_ALU.is_free();
        case compare:
            return Compare_ALU.is_free();
    }
}

void all_ALU::flush() {
    Logic_ALU.flush();
    Add_ALU.flush();
    Shift_ALU.flush();
    Compare_ALU.flush();
}

int all_ALU::execute(int instruction, int operand_one, int operand_two) {
    switch (get_ALU_unit(instruction)) {
        case logic:
            return Logic_ALU.execute(instruction, operand_one, operand_two);
        case add:
            return Add_ALU.execute(instruction, operand_one, operand_two);
        case shift:
            return Shift_ALU.execute(instruction, operand_one, operand_two);
        case compare:
            return Compare_ALU.execute(instruction, operand_one, operand_two);
    }
}