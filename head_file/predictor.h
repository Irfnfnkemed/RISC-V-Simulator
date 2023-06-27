#ifndef RISC_V_SIMULATOR_PREDICTOR_H
#define RISC_V_SIMULATOR_PREDICTOR_H

class predictor {
private:
    //两位计数器，二进制下，10、11跳转，00、01不跳
    int counter = 0;
    int counter_next = 0;
public:
    //初始化
    void init();

    //预测是否跳转
    bool jump();

    //更改计数器，预测正确加1；错误减1
    void change_counter(bool is_correct);

    //刷新
    void flush();
};


#endif //RISC_V_SIMULATOR_PREDICTOR_H
