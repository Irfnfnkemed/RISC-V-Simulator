#ifndef RISC_V_SIMULATOR_ALGORITHM_LOGIC_UNIT_H
#define RISC_V_SIMULATOR_ALGORITHM_LOGIC_UNIT_H

#include "tool.h"

class base_ALU {
protected:
    int free = 2;//支持两条运算同时进行
public:
    //是否仍有空闲
    bool is_free();

    //刷新
    void flush();

    //设置操作名和操作数，对ALU指令进行操作，并输出结果
    virtual int execute(int instruction, int operand_one, int operand_two) = 0;
};


//逻辑运算
class logic_ALU : public base_ALU {
public:
    int execute(int instruction, int operand_one, int operand_two);
};

//算术运算
class add_ALU : public base_ALU {
public:
    int execute(int instruction, int operand_one, int operand_two);
};

//位运算
class shift_ALU : public base_ALU {
public:
    int execute(int instruction, int operand_one, int operand_two);
};

//比较运算
class compare_ALU : public base_ALU {
public:
    int execute(int instruction, int operand_one, int operand_two);
};


class address_ALU : public base_ALU {
public:
    //计算地址
    int execute(int addr, int offset, int useless = 0);
};

class all_ALU {
private:
    enum cate {
        logic, add, shift, compare
    };

    logic_ALU Logic_ALU;
    add_ALU Add_ALU;
    shift_ALU Shift_ALU;
    compare_ALU Compare_ALU;

    //得到对应ALU的种类
    int get_ALU_unit(int instruction);

public:
    bool is_free(int instruction);

    void flush();

    int execute(int instruction, int operand_one, int operand_two);
};

#endif //RISC_V_SIMULATOR_ALGORITHM_LOGIC_UNIT_H