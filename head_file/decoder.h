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
        int instr = -1;//为-1时表示指令无效
        int reg_one = -1, reg_two = -1;//两个寄存器的编号
        int imd = -1;//用于存放立即数
        int dest = -1;//目标寄存器的编号(若存在)


        int pc;
    };

    decode_instr instr_decode;//解码后指令
    decode_instr instr_decode_next;//缓冲区
    bool freeze = false;

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
    void fetch_instr(int &instr_, int &reg_one_, int &reg_two_, int &imd_, int &dest_, int &pc);

    //刷新
    void flush();

    //分支预测错误，清除所有的数据
    void clear();

    void ppp();
};


#endif //RISC_V_SIMULATOR_DECODER_H