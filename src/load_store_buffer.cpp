#include "load_store_buffer.h"
#include "reorder_buffer.h"

void load_store_buffer::load_store_memory() {
    if (clock_time > 0) {//正常进行读写进程
        --clock_time;
        if (!clock_time) {//时钟数清零，表明某个内存读写操作已经完成
            load_store_unit task = buffer_next.pop();
            switch (task.instruction) {
                case LB:
                    task.value_two = sign_extend(MEM->load_memory(task.value_one, 1), 8);
                    break;
                case LH:
                    task.value_two = sign_extend(MEM->load_memory(task.value_one, 2), 16);
                    break;
                case LW:
                    task.value_two = MEM->load_memory(task.value_one, 4);
                    break;
                case LBU:
                    task.value_two = MEM->load_memory(task.value_one, 1);
                    break;
                case LHU:
                    task.value_two = MEM->load_memory(task.value_one, 2);
                    break;
                case SB:
                    MEM->store_memory(task.value_one, task.value_two, 1);
                    break;
                case SH:
                    MEM->store_memory(task.value_one, task.value_two, 2);
                    break;
                case SW:
                    MEM->store_memory(task.value_one, task.value_two, 4);
            }
            if (task.instruction >= LB && task.instruction <= LHU) {//将load指令返回ROB
                return_to_ROB(task.tag, task.value_two);
            }
        }
    }
}

void load_store_buffer::set_task() {
    if (!clock_time && !buffer.empty() && buffer.front().ready) { clock_time = 3; }
}

void load_store_buffer::return_to_ROB(int tag_, int data_) { ROB->set_ready(tag_, data_); }

void load_store_buffer::init(memory *MEM_, reorder_buffer *ROB_) {
    MEM = MEM_;
    ROB = ROB_;
}

void load_store_buffer::add_instruction(int tag_) {
    buffer_next.push(load_store_unit{1, 0, 0, tag_, false});
}

void load_store_buffer::update_data(int instruction_, int value_one_, int value_two_, int tag_) {
    for (auto iter = buffer_next.begin(); iter != buffer_next.end(); ++iter) {
        if (iter->tag == tag_) {
            iter->instruction = instruction_;
            iter->value_one = value_one_;
            iter->value_two = value_two_;
            iter->ready = true;
        }
    }
}

bool load_store_buffer::empty() { return buffer.empty(); }

void load_store_buffer::execute() {
    set_task();
    load_store_memory();
}

void load_store_buffer::clear() {
    for (auto iter = buffer_next.begin(); iter != buffer_next.end(); ++iter) {
        if (iter->instruction >= SB && iter->instruction <= SW && iter->ready) {
            buffer.push(*iter);
        }
    }
    buffer_next = buffer;
}

void load_store_buffer::flush() { buffer = buffer_next; }
