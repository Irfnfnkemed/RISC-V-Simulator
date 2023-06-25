#include "../head_file/load_store_buffer.h"

void load_store_buffer::init(memory *Memory_, reorder_buffer *Reorder_buffer_) {
    Memory = Memory_;
    Reorder_buffer = Reorder_buffer_;
}

void load_store_buffer::add_instruction(int instruction_, int value_one_, int value_two_,
                                        int depend_one_, int depend_two_,
                                        int offset, int destination_, int tag_) {
    buffer.push(load_store_unit{instruction_ - OFFSET, value_one_, value_two_,
                                depend_one_, depend_two_,
                                offset, destination_, tag_, false});
}

void load_store_buffer::execute_rising_edge() {
    for (auto iter = buffer.begin(); iter != buffer.end(); ++iter) {
        if (!iter->execute && iter->depend_one == -1 && iter->depend_two == -1) {//计算地址
            iter->value_one = address_ALU.execute(iter->value_one, iter->offset);
            iter->execute = true;
        }
    }
}

void load_store_buffer::execute_falling_edge() {
    if (clock_time > 0) {//正常进行读写进程
        --clock_time;
        if (!clock_time) {//时钟数清零，表明某个内存读写操作已经完成
            load_store_unit &task = buffer.front();
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
            //////////////////////// Reorder_buffer->set_ready(tag_);
        }
    }
    //设置下一个读写任务
    if (!clock_time && !buffer.empty() && buffer.front().execute) { clock_time = 3; }
}


void load_store_buffer::response_CDB(int tag_, int reg_, int data_) {
    for (auto iter = buffer.begin(); iter != buffer.end(); ++iter) {
        if (iter->depend_one == tag_) {
            if (iter->value_one == reg_) { iter->value_one = data_; }
            iter->depend_one = -1;
        } else if (iter->depend_two == reg_) {
            if (iter->depend_two == reg_) { iter->value_two = data_; }
            iter->depend_two = -1;
        }
    }
}
