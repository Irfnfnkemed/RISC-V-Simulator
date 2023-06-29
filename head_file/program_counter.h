#ifndef RISC_V_SIMULATOR_PROGRAM_COUNTER_H
#define RISC_V_SIMULATOR_PROGRAM_COUNTER_H

class program_counter {
private:

    int pc;
    int offset = 4;
    bool begin = false;//begin为真表示更新时直接将PC设为偏移量，反之PC加上偏移量
    bool clean = false;//周期末程序是否会清空
    bool stop = false;//PC是否停下
    bool stop_next = false;//缓冲
    bool freeze = false;//ROB已满，PC及Decoder保持不变

public:
    //初始化
    void init();

    //设置偏移量(begin_为真表示直接赋为offset，反之+=offset)(clean_为真表示周期末程序会清空)
    void set_offset(int offset_, bool begin_ = false, bool clean_ = false);

    //ROB已满，设置PC及Decoder保持不变
    void set_freeze();

    //是否停下
    bool is_stop();

    //得到PC指向
    int get_pc();

    //设置PC是否停止
    void set_stop(bool stop_);

    //更新PC指向
    void flush();

    //恢复PC正常状态
    void clear();
};

#endif //RISC_V_SIMULATOR_PROGRAM_COUNTER_H
