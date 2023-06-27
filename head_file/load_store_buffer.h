#ifndef RISC_V_SIMULATOR_LOAD_STORE_BUFFER_H
#define RISC_V_SIMULATOR_LOAD_STORE_BUFFER_H

#include "memory.h"
#include "tool.h"

class reorder_buffer;

class load_store_buffer {
private:

    struct load_store_unit {
        int instruction = -1;//load_store_buffer相关的指令编号，空时为-1
        int value_one, value_two;//两个相关寄存器的值
        int tag;//读写指令在ROB中对应的tag
        bool ready = false;
    };

    queue<load_store_unit, 64> buffer;//循环队列
    queue<load_store_unit, 64> buffer_next;//缓冲区
    int clock_time = 0;
    memory *MEM;
    reorder_buffer *ROB;

    //内存读写进程
    void load_store_memory();

    //设置下一个读写任务
    void set_task();

    //完成读操作后返回至ROB
    void return_to_ROB(int tag_, int data_);

public:

    //初始化，关联相关模块
    void init(memory *MEM_, reorder_buffer *ROB_);

    //添加任务(占位，尚不可执行)
    void add_instruction(int tag_);

    //上传数据(现可执行)
    void update_data(int instruction_, int value_one_, int value_two_, int tag_);

    //队列是否为空
    bool empty();

    //执行函数
    void execute();

    //分支预测错误，清除所有的load指令和未ready的store指令，重整队列
    void clear();

    //将缓冲区更新至当前状态
    void flush();

};

#endif //RISC_V_SIMULATOR_LOAD_STORE_BUFFER_H
