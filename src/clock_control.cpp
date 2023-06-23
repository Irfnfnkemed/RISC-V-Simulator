#include "../head_file/clock_control.h"

void clock_control::init_set() {
    Memory.init();
    Load_store_buffer.init(&Memory);
}