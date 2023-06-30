#include "clock_control.h"

void clock_control::init() {
    clock = 0;
    to_be_cleared = to_be_finished = false;
    return_value = -1;
    MEM.init();
    ROB.init(&RS, &LSB, &RF, &PC, &Decoder, &PRE);
    RS.init(&ROB, &LSB);
    LSB.init(&MEM, &ROB);
    RF.init();
    PC.init();
    Decoder.init(&PC, &MEM, &PRE);
    PRE.init();
}

void clock_control::execute() {
    ROB.execute(to_be_cleared, a);
    RS.execute();
    LSB.execute();
    Decoder.execute(to_be_finished);
}

void clock_control::flush() {
    ++clock;
    if (to_be_cleared) {
        ROB.clear();
        RS.clear();
        LSB.clear();
        RF.clear();
        Decoder.clear();
        PC.clear();
        to_be_cleared = false;
    } else {
        ROB.flush();
        RS.flush();
        LSB.flush();
        RF.flush();
        PC.flush();
        Decoder.flush();
        PRE.flush();
        if (to_be_finished) {
            if (LSB.empty()) { return_value = RF.get_return_value(); }
        }
    }
//    if (a) {
//        RF.aaa();///
//        std::cout << "clock= " << clock << "\n-----------------\n";
//    }
    a = false;
}

int clock_control::finish() { return return_value; }