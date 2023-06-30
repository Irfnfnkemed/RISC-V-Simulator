#include "predictor.h"

void predictor::init() { counter = counter_next = correct = incorrect = 0; }

bool predictor::jump() { return counter >= 2; }

void predictor::change_counter(bool is_jump, bool is_correct) {
    if (is_correct) { ++correct; }
    else { ++incorrect; }
    if (is_jump) {
        if (counter < 3) { ++counter_next; }
    } else {
        if (counter > 0) { --counter_next; }
    }
}

double predictor::get_correct_rate() { return (double) correct / (correct + incorrect); }

void predictor::flush() { counter = counter_next; }