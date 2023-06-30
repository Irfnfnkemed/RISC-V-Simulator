#include "clock_control.h"

int main() {
    clock_control RISCV;
    RISCV.init();
    while (!RISCV.finish()) {
        RISCV.execute();
        RISCV.flush();
    }
    //std::cout << RISCV.get_predictor_correct_rate() << std::endl;
    std::cout << RISCV.get_return_value() << std::endl;
    return 0;
}