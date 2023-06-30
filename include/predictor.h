#ifndef RISC_V_SIMULATOR_PREDICTOR_H
#define RISC_V_SIMULATOR_PREDICTOR_H

#include "tool.h"

class predictor {
private:
    //两位计数器，二进制下，10、11跳转，00、01不跳
    u_int_2 counter[256];
    int correct = 0;//正确次数
    int incorrect = 0;//错误次数
public:
    //初始化
    void init();

    //预测是否跳转
    bool jump(u_int8_t id) const;

    //更改计数器，预测正确加1；错误减1；同时维护正确、错误次数。is_jump表示真实情况是否跳转
    void change_counter(u_int8_t id, bool is_jump, bool is_correct);

    //得到预测正确率
    double get_correct_rate() const;
};


#endif //RISC_V_SIMULATOR_PREDICTOR_H
