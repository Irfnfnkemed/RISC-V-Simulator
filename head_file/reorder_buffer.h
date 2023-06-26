#ifndef RISC_V_SIMULATOR_REORDER_BUFFER_H
#define RISC_V_SIMULATOR_REORDER_BUFFER_H

#include "algorithm_logic_unit.h"
#include "tool.h"

class reservation_station;

class load_store_buffer;

class register_file;

class program_counter;

class reorder_buffer {
private:

    struct reorder_buffer_unit {
        int tag;//标签，用时钟数表示
        int instr;
        int reg_one, reg_two;//两个寄存器的编号
        int imd;//用于存放立即数(以及RS/load_store_buffer返回的结果值)
        int dest;//目标寄存器的编号(若存在)
        bool launch = false;//是否发射
        bool ready = false;//可否提交
    };

    address_ALU Address_ALU;
    reservation_station *RS;
    load_store_buffer *LSB;
    register_file *RF;
    program_counter *PC;
    queue<reorder_buffer_unit, 16> buffer;
    queue<reorder_buffer_unit, 16> buffer_next;//缓存

    //在RF中添加标记
    void set_tag(int instr_, int tag_, int dest_);

    //发射指令
    void launch();

    //提交指令
    void commit();

public:
    void init(reservation_station *RS_, load_store_buffer *LSB_,
              register_file *RF_, program_counter *PC_);

    void execute();

    //将对应指令设为ready状态，将返回数据存到立即数中
    void set_ready(int tag_, int data_);

    //刷新
    void flush();
}

#endif //RISC_V_SIMULATOR_REORDER_BUFFER_H
