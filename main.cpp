#include "clock_control.h"

int main() {
    clock_control RISCV;
    RISCV.init();
    while (!RISCV.finish()) {
        RISCV.execute();
        RISCV.flush();
    }
    std::cout << std::dec << RISCV.get_return_value() << std::endl;
    return 0;
}