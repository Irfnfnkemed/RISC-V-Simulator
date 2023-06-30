#ifndef RISC_V_SIMULATOR_CLOCK_CONTROL_H
#define RISC_V_SIMULATOR_CLOCK_CONTROL_H

#include "memory.h"
#include "reorder_buffer.h"
#include "reservation_staion.h"
#include "load_store_buffer.h"
#include "register_file.h"
#include "program_counter.h"
#include "decoder.h"
#include "predictor.h"

class clock_control {
private:
    memory MEM;
    reorder_buffer ROB;
    reservation_station RS;
    load_store_buffer LSB;
    register_file RF;
    program_counter PC;
    decoder Decoder;
    predictor PRE;

    bool to_be_cleared = false;
    bool to_be_finished = false;
    int return_value = -1;
    int clock = 0;

public:

    //初始化
    void init();

    //执行
    void execute();

    //刷新
    void flush();

    //程序是否结束
    bool finish();

    //返回程序结束值
    int get_return_value();

    //得到分支预测正确率
    double get_predictor_correct_rate();

};

#endif //RISC_V_SIMULATOR_CLOCK_CONTROL_H