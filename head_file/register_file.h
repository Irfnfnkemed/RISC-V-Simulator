#ifndef RISC_V_SIMULATOR_REGISTER_FILE_H
#define RISC_V_SIMULATOR_REGISTER_FILE_H

class register_file {
private:

    int register_unit[32];
    int depend[32];//存储依赖的指令在ROB中的tag，无关系时为-1

public:

    void init();

    int get_depend(int reg_);

    int get_value(int reg_);

    //监听CDB
    void response_CDB(int tag_, int reg_, int data_);

};

#endif //RISC_V_SIMULATOR_REGISTER_FILE_H
