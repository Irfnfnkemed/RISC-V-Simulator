#include "../head_file/load_store_buffer.h"

void load_store_buffer::init(memory *Memory_) { Memory = Memory_; }

void load_store_buffer::push_buffer(int instruction_, int addr_, int data_, int tag_) {
    buffer.push(load_store_unit{instruction_ - OFFSET, addr_, data_, tag_});
}

void load_store_buffer::execute() {
    if (clock_time > 0) {
        --clock_time;
        if (!clock_time) {//时钟数清零，表明某个内存读写操作已经完成
            load_store_unit pop = buffer.pop();
            switch (pop.instruction) {
                case LB:
                    pop.data = sign_extend(Memory->load_memory(pop.addr, 1), 8);
                    break;
                case LH:
                    pop.data = sign_extend(Memory->load_memory(pop.addr, 2), 16);
                    break;
                case LW:
                    pop.data = Memory->load_memory(pop.addr, 4);
                    break;
                case LBU:
                    pop.data = Memory->load_memory(pop.addr, 1);
                    break;
                case LHU:
                    pop.data = Memory->load_memory(pop.addr, 2);
                    break;
                case SB:
                    Memory->store_memory(pop.addr, pop.data, 1);
                    break;
                case SH:
                    Memory->store_memory(pop.addr, pop.data, 2);
                    break;
                case SW:
                    Memory->store_memory(pop.addr, pop.data, 4);
            }
        }
    }
    if (!clock_time && !buffer.empty()) {//设置下一个读写任务
        clock_time = 3;
    }
}