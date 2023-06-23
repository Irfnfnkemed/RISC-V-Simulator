#ifndef RISC_V_SIMULATOR_CLOCK_CONTROL_H
#define RISC_V_SIMULATOR_CLOCK_CONTROL_H

#include "memory.h"
#include "load_store_buffer.h"

class clock_control {
private:
    memory Memory;
    load_store_buffer Load_store_buffer;
public:
    void init_set();

};

#endif //RISC_V_SIMULATOR_CLOCK_CONTROL_H
