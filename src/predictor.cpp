#include "predictor.h"

void predictor::init() {
    for (int i = 0; i < 256; ++i) { counter[i] = u_int_2{false, true}; }
    correct = incorrect = 0;
}

bool predictor::jump(u_int8_t id) const { return counter[id].fetch_high(); }

void predictor::change_counter(u_int8_t id, bool is_jump, bool is_correct) {
    if (is_correct) { ++correct; }
    else { ++incorrect; }
    if (is_jump) { ++counter[id]; }
    else { --counter[id]; }
}

double predictor::get_correct_rate() const { return (double) correct / (correct + incorrect); }