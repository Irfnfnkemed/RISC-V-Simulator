#ifndef RISC_V_SIMULATOR_DECODER_H
#define RISC_V_SIMULATOR_DECODER_H

#include "tool.h"
#include "memory.h"

class program_counter;

class reorder_buffer;

class predictor;

class decoder {
private:
    program_counter *PC;
    memory *MEM;
    predictor *PRE;

    struct decode_instr {
        u_int8_t instr = 0xff;//为0xff时表示指令无效
        u_int8_t reg_one = 0xff, reg_two = 0xff;//两个寄存器的编号
        int imd = -1;//用于存放立即数
        u_int8_t dest = 0xff;//目标寄存器的编号(若存在)
        int other;//存放其他数
    };

    decode_instr instr_decode;//解码后指令
    decode_instr instr_decode_next;//缓冲区
    bool freeze = false;//为真时，设置为不变状态(持续一个周期)

    //解码
    void decode(int instr_bin, decode_instr &out, bool &to_be_finished);


public:

    //初始化
    void init(program_counter *PC_, memory *MEM_, predictor *PRE_);

    //是否已经发送至ROB
    bool is_send();

    //执行
    void execute(bool &to_be_finished);

    //获得解码后指令
    void fetch_instr(u_int8_t &instr_, u_int8_t &reg_one_, u_int8_t &reg_two_,
                     int &imd_, u_int8_t &dest_, int &other);

    //设置为不变状态
    void set_freeze();

    //刷新
    void flush();

    //分支预测错误，清除所有的数据
    void clear();
};


#endif //RISC_V_SIMULATOR_DECODER_H