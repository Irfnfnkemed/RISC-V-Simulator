#include "tool.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <memory>

int hex_to_dec(const char source[], int digit) {
    int dec_number = 0;
    for (int i = 0; i < digit; ++i) {
        dec_number = dec_number << 4;
        if (source[i] >= 'A' && source[i] <= 'F') { dec_number += source[i] - 'A' + 10; }
        else { dec_number += source[i] - '0'; }
    }
    return dec_number;
}

int sign_extend(int source, int digit) {
    if (digit < 32 && source >> (digit - 1)) {
        int fetch = -(1 << digit);
        source = source | fetch;
    }
    return source;
}

int fetch(int source, int high, int low) {
    int fetch_num = (1 << (high - low + 1)) - 1;
    return (source >> low) & fetch_num;
}

int shuffle(int order[], int length) {
    srand(time(nullptr));
    for (int i = 0; i < length; ++i) { order[i] = i + 1; }
    std::random_shuffle(order, order + length);
}

void print_the_instruction(int instruction) {
    switch (instruction) {
        case LUI:
            std::cout << "LUI" << std::endl;
            break;
        case AUIPC:
            std::cout << "AUIPC" << std::endl;
            break;
        case JAL:
            std::cout << "JAL" << std::endl;
            break;
        case JALR:
            std::cout << "JALR" << std::endl;
            break;
        case BEQ:
            std::cout << "BEQ" << std::endl;
            break;
        case BNE:
            std::cout << "BNE" << std::endl;
            break;
        case BLT:
            std::cout << "BLT" << std::endl;
            break;
        case BGE:
            std::cout << "BGE" << std::endl;
            break;
        case BLTU:
            std::cout << "BLTU" << std::endl;
            break;
        case BGEU:
            std::cout << "BGEU" << std::endl;
            break;
        case LB:
            std::cout << "LB" << std::endl;
            break;
        case LH:
            std::cout << "LH" << std::endl;
            break;
        case LW:
            std::cout << "LW" << std::endl;
            break;
        case LBU:
            std::cout << "LBU" << std::endl;
            break;
        case LHU:
            std::cout << "LHU" << std::endl;
            break;
        case SB:
            std::cout << "SB" << std::endl;
            break;
        case SH:
            std::cout << "SH" << std::endl;
            break;
        case SW:
            std::cout << "SW" << std::endl;
            break;
        case ADDI:
            std::cout << "ADDI" << std::endl;
            break;
        case SLTI:
            std::cout << "SLTI" << std::endl;
            break;
        case SLTIU:
            std::cout << "SLTIU" << std::endl;
            break;
        case XORI:
            std::cout << "XORI" << std::endl;
            break;
        case ORI:
            std::cout << "ORI" << std::endl;
            break;
        case ANDI:
            std::cout << "ANDI" << std::endl;
            break;
        case SLLI:
            std::cout << "SLLI" << std::endl;
            break;
        case SRLI:
            std::cout << "SRLI" << std::endl;
            break;
        case SRAI:
            std::cout << "SRAI" << std::endl;
            break;
        case ADD:
            std::cout << "ADD" << std::endl;
            break;
        case SUB:
            std::cout << "SUB" << std::endl;
            break;
        case SLL:
            std::cout << "SLL" << std::endl;
            break;
        case SLT:
            std::cout << "SLT" << std::endl;
            break;
        case SLTU:
            std::cout << "SLTU" << std::endl;
            break;
        case XOR:
            std::cout << "XOR" << std::endl;
            break;
        case SRL:
            std::cout << "SRL" << std::endl;
            break;
        case SRA:
            std::cout << "SRA" << std::endl;
            break;
        case OR:
            std::cout << "OR" << std::endl;
            break;
        case AND:
            std::cout << "AND" << std::endl;
            break;
        case END:
            std::cout << "END" << std::endl;
            break;
    }
}

u_int_2::u_int_2(bool num_high, bool num_low) {
    num[0] = num_high;
    num[1] = num_low;
}

u_int_2 &u_int_2::operator++() {
    if (num[0] && num[1]) { return *this; }
    if (num[1]) { num[0] = true; }
    num[1] = !num[1];
    return *this;
}

u_int_2 &u_int_2::operator--() {
    if (!num[0] && !num[1]) { return *this; }
    if (!num[1]) { num[0] = false; }
    num[1] = !num[1];
    return *this;
}

u_int_2 &u_int_2::operator=(u_int_2 const &obj) {
    if (this == &obj) { return *this; }
    num[0] = obj.num[0];
    num[1] = obj.num[1];
    return *this;
}

bool u_int_2::fetch_high() const { return num[0]; }