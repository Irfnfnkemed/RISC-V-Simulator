#include "reservation_staion.h"
#include "reorder_buffer.h"
#include "load_store_buffer.h"

void reservation_station::calculate() {
    for (int i = 0; i < 16; ++i) {
        if (buffer[i].instr >= 0) {
            if (buffer[i].instr == JALR ||
                (buffer[i].instr >= LB && buffer[i].instr <= LHU) ||
                (buffer[i].instr >= ADDI && buffer[i].instr <= SRAI)) {//有一个立即数的操作
                if (ALU.is_free(buffer[i].instr) && buffer[i].depend_one == -1) {
                    buffer_next[i].imd = ALU.execute(buffer[i].instr, buffer[i].value_one, buffer[i].imd);
                    return_to_ROB_or_LSB(buffer[i].instr, buffer[i].tag, buffer_next[i].imd);
                    buffer_next[i].instr = -1;//删去
                }
            } else {//两个寄存器的操作
                if (ALU.is_free(buffer[i].instr) && buffer[i].depend_one == -1 && buffer[i].depend_two == -1) {
                    buffer_next[i].imd = ALU.execute(buffer[i].instr, buffer[i].value_one, buffer[i].value_two);
                    return_to_ROB_or_LSB(buffer[i].instr, buffer[i].tag, buffer_next[i].imd);
                    buffer_next[i].instr = -1;//删去
                }
            }
        }
    }
}

void reservation_station::return_to_ROB_or_LSB(int instr_, int tag_, int output_) {
    if (instr_ >= LB && instr_ <= LHU) { LSB->update_data(instr_, output_, -1, tag_); }
    else { ROB->set_ready(tag_, output_); }
}

void reservation_station::flush_depend(int tag_, int data_) {
    for (int i = 0; i < 16; ++i) {
        if (buffer_next[i].instr != -1) {
            if (buffer_next[i].depend_one == tag_) {
                buffer_next[i].value_one = data_;
                buffer_next[i].depend_one = -1;
            } else if (buffer_next[i].depend_two == tag_) {
                buffer_next[i].value_two = data_;
                buffer_next[i].depend_two = -1;
            }
        }
    }
}

void reservation_station::init(reorder_buffer *ROB_, load_store_buffer *LSB_) {
    ROB = ROB_;
    LSB = LSB_;
}

void reservation_station::add_instruction(int instr_, int value_one_, int value_two_,
                                          int depend_one_, int depend_two_, int imd_, int tag_) {
    int index = 0;
    while (buffer[index].instr != -1) { ++index; }
    buffer_next[index] = reservation_unit{instr_, value_one_, value_two_,
                                          depend_one_, depend_two_, imd_, tag_};
}

void reservation_station::execute() { calculate(); }


void reservation_station::flush() {
    for (int i = 0; i < 16; ++i) { buffer[i] = buffer_next[i]; }
}

void reservation_station::clear() {
    for (int i = 0; i < 16; ++i) { buffer[i] = buffer_next[i] = reservation_unit(); }
}