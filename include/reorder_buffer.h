#ifndef RISC_V_SIMULATOR_REORDER_BUFFER_H
#define RISC_V_SIMULATOR_REORDER_BUFFER_H

#include "algorithm_logic_unit.h"

class reservation_station;

class load_store_buffer;

class register_file;

class program_counter;

class decoder;

class predictor;

class reorder_buffer {
private:

    struct reorder_buffer_unit {
        int tag;//标签
        u_int8_t instr = 0xff;
        u_int8_t reg_one, reg_two;//两个寄存器的编号
        int imd;//用于存放立即数(以及RS/load_store_buffer返回的结果值)
        u_int8_t dest;//目标寄存器的编号(若存在)
        int output;//从RS/address_ALU返回的值
        bool launch = false;//是否发射
        bool ready = false;//可否提交
    };


    address_ALU Address_ALU;
    reservation_station *RS;
    load_store_buffer *LSB;
    register_file *RF;
    program_counter *PC;
    decoder *Decoder;
    predictor *PRE;
    queue<reorder_buffer_unit, 16> buffer;
    queue<reorder_buffer_unit, 16> buffer_next;//缓存

    //在RF中添加标记
    void set_tag(u_int8_t instr_, int tag_, u_int8_t dest_);

    //得到依赖关系和数据
    void get(u_int8_t reg_, int &depend_, int &value_);

    //发射指令
    void launch();

    //提交指令
    void commit(bool &to_be_cleared);

    //从Decoder中获得一条指令
    void add_instruction();

public:
    //初始化
    void init(reservation_station *RS_, load_store_buffer *LSB_,
              register_file *RF_, program_counter *PC_,
              decoder *Decoder_, predictor *PRE_);

    //执行
    void execute(bool &to_be_cleared);

    //将对应指令设为ready状态，将返回数据存到立即数中
    void set_ready(int tag_, int data_);

    //刷新
    void flush();

    //分支预测错误，清除所有数据
    void clear();
};

#endif //RISC_V_SIMULATOR_REORDER_BUFFER_H
