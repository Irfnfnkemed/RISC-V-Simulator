#ifndef RISC_V_SIMULATOR_REGISTER_FILE_H
#define RISC_V_SIMULATOR_REGISTER_FILE_H

#include "tool.h"

class register_file {
private:

    int register_unit[32];//寄存器的值
    int depend[32];//存储依赖的指令在ROB中的tag，无关系时为-1
    int register_unit_next[32];//缓冲区
    int depend_next[32];//缓冲区
    bool modify[32];//是否添加了新的tag，每周期更新

public:

    //初始化
    void init();

    //获得寄存器的依赖关系和值
    void get(u_int8_t reg_, int &depend_, int &value);

    //获得目标寄存器的值（仅用于store的commit，不检查依赖关系）
    int get(u_int8_t reg_);

    //指令完成提交后，删除相关标记，并更新其值
    void flush_tag(int tag_, int data_, u_int8_t reg_);

    //给reg_寄存器存标记
    void set_tag(int tag_, u_int8_t reg_);

    //得到a10的返回值
    int get_return_value();

    //刷新
    void flush();

    //分支预测错误，清除所有依赖关系
    void clear();
};

#endif //RISC_V_SIMULATOR_REGISTER_FILE_H
