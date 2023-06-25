#include "../head_file/reservation_staion.h"

void reservation_station::init(reorder_buffer *Reorder_buffer_) {
    Reorder_buffer = Reorder_buffer_;
}

void reservation_station::add_instruction(int instruction_, int value_one_, int value_two_,
                                          int depend_one_, int depend_two_,
                                          int immediate_, int destination_, int tag_) {
    int index = 0;
    while (buffer[index].status != 0) { ++index; }
    buffer[index] = reservation_unit{instruction_ - OFFSET, value_one_, value_two_,
                                     depend_one_, depend_two_, immediate_, destination_, tag_, 0};
}

void reservation_station::execute_rising_edge() {
    for (int i = 0; i < 16; ++i) {
        if (buffer[i].instruction >= 0 && buffer[i].status == 0) {
            if (buffer[i].instruction < 9) {
                if (buffer[i].depend_one == -1) {
                    buffer[i].immediate = ALU.execute(buffer[i].instruction,
                                                      buffer[i].value_one, buffer[i].immediate);
                    buffer[i].status = 1;
                }
            } else {
                if (buffer[i].depend_one == -1 && buffer[i].depend_one == -1) {
                    buffer[i].immediate = ALU.execute(buffer[i].instruction,
                                                      buffer[i].value_one, buffer[i].value_two);
                    buffer[i].status = 1;
                }
            }
        }
    }
}

void reservation_station::execute_falling_edge() {
    for (int i = 0; i < 16; ++i) {
        if (buffer[i].status == 1) {
            //////////////////////// Reorder_buffer->set_ready(tag_);
        }
    }
}

void reservation_station::response_CDB(int tag_, int reg_, int data_) {
    for (int i = 0; i < 16; ++i) {
        if (buffer[i].tag == tag_) {
            buffer[i].instruction = -1; //设为空闲
            if (buffer[i].depend_one == reg_) {
                buffer[i].depend_one = -1;
                buffer[i].value_one = data_;
            } else if (buffer[i].depend_two == reg_) {
                buffer[i].depend_two = -1;
                buffer[i].value_two = data_;
            }
        }
    }
}