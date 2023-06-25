#include "../head_file/load_store_buffer.h"


void load_store_buffer::load_store_memory() {
    if (clock_time > 0) {//正常进行读写进程
        --clock_time;
        if (!clock_time) {//时钟数清零，表明某个内存读写操作已经完成
            load_store_unit task = buffer_next.pop();
            switch (task.instruction) {
                case LB:
                    task.value_two = sign_extend(Memory->load_memory(task.value_one, 1), 8);
                    break;
                case LH:
                    task.value_two = sign_extend(Memory->load_memory(task.value_one, 2), 16);
                    break;
                case LW:
                    task.value_two = Memory->load_memory(task.value_one, 4);
                    break;
                case LBU:
                    task.value_two = Memory->load_memory(task.value_one, 1);
                    break;
                case LHU:
                    task.value_two = Memory->load_memory(task.value_one, 2);
                    break;
                case SB:
                    Memory->store_memory(task.value_one, task.value_two, 1);
                    break;
                case SH:
                    Memory->store_memory(task.value_one, task.value_two, 2);
                    break;
                case SW:
                    Memory->store_memory(task.value_one, task.value_two, 4);
            }
            if (task.instruction >= LB && task.instruction <= LHU) {//将load指令返回ROB
                ROB->set_ready(task.tag, task.value_two);
            }
        }
    }
}

void load_store_buffer::set_task() {
    if (!clock_time && !buffer.empty()) { clock_time = 3; }
}

void load_store_buffer::init(memory *Memory_, reorder_buffer *ROB_) {
    Memory = Memory_;
    ROB = ROB_;
}

void load_store_buffer::add_instruction(int instruction_, int value_one_, int value_two_, int tag_) {
    buffer_next.push(load_store_unit{instruction_, value_one_, value_two_, tag_});
}

void load_store_buffer::execute() {
    load_store_memory();
    set_task();
}

void load_store_buffer::flush() {
    buffer = buffer_next;
}
