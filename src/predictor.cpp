#include "predictor.h"

void predictor::init() { counter = counter_next = 0; }

bool predictor::jump() { return counter >= 2; }

void predictor::change_counter(bool is_correct) {
    if (is_correct) {
        if (counter < 3) { ++counter_next; }
    } else {
        if (counter > 0) { --counter_next; }
    }
}

void predictor::flush() { counter = counter_next; }