#ifndef RISC_V_SIMULATOR_RESERVATION_STATION_H
#define RISC_V_SIMULATOR_RESERVATION_STATION_H

#include "algorithm_logic_unit.h"
#include "tool.h"

class reorder_buffer;

class load_store_buffer;

class reservation_station {
private:

    struct reservation_unit {
        int instr = -1;//指令编号，为-1时表示该位置为空
        int value_one, value_two;//两个寄存器(若存在)的值
        int depend_one = -1, depend_two = -1;//两个寄存器(若存在)对前面指令的依赖关系，存储依赖指令的tag，无依赖时默认为-1
        int imd;//存放立即数(或者运算结果)
        int tag;//指令在ROB中的tag
    };

    all_ALU ALU;
    reorder_buffer *ROB;
    load_store_buffer *LSB;

    reservation_unit buffer[16];
    reservation_unit buffer_next[16];//缓冲区

    //将可执行的放入ALU中进行运算，将结果返回ROB/LSB，并在RS中删去
    void calculate();

    //将算完的任务返回到ROB或LSB(load指令)
    void return_to_ROB_or_LSB(int instr_, int tag_, int data_);

public:

    //初始化
    void init(reorder_buffer *ROB_, load_store_buffer *LSB_);

    //删去标签为tag_对应的寄存器依赖关系，并修改其值
    void flush_depend(int tag_, int output_);

    //添加任务
    void add_instruction(int instr_, int value_one_, int value_two_,
                         int depend_one_, int depend_two_, int imd_, int tag_);

    //执行函数
    void execute();

    //刷新
    void flush();

    //分支预测错误，清除所有数据
    void clear();

};

#endif //RISC_V_SIMULATOR_RESERVATION_STATION_H
