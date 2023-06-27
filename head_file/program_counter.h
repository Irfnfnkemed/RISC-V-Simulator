#ifndef RISC_V_SIMULATOR_PROGRAM_COUNTER_H
#define RISC_V_SIMULATOR_PROGRAM_COUNTER_H

class program_counter {
private:

    int pc;
    int offset = 4;
    bool begin;//begin为真表示更新时直接将PC设为偏移量，反之PC加上偏移量
    bool stop = false;//PC是否停下

public:
    //初始化
    void init();

    //设置偏移量
    void set_offset(int offset_, bool begin_ = false);

    //是否停下
    bool is_stop();

    //得到PC指向
    int get_pc();

    //设置PC是否停止
    void set_stop(bool stop_);

    //更新PC指向
    void flush();
};

#endif //RISC_V_SIMULATOR_PROGRAM_COUNTER_H
