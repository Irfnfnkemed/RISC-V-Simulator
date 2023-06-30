#include "clock_control.h"

void clock_control::init() {
    clock = 0;
    to_be_cleared = to_be_finished = false;
    return_value = -1;
    int order[8];
    shuffle(order, 8);
    for (int i = 0; i < 8; ++i) {
        switch (order[i]) {
            case 1:
                MEM.init();
                break;
            case 2:
                ROB.init(&RS, &LSB, &RF, &PC, &Decoder, &PRE);
                break;
            case 3:
                RS.init(&ROB, &LSB);
                break;
            case 4:
                LSB.init(&MEM, &ROB);
                break;
            case 5:
                RF.init();
                break;
            case 6:
                PC.init();
                break;
            case 7:
                Decoder.init(&PC, &MEM, &PRE);
                break;
            case 8:
                PRE.init();
        }
    }
}

void clock_control::execute() {
    int order[4];
    shuffle(order, 4);
    for (int i = 0; i < 4; ++i) {
        switch (order[i]) {
            case 1:
                ROB.execute(to_be_cleared);
                break;
            case 2:
                RS.execute();
                break;
            case 3:
                LSB.execute();
                break;
            case 4:
                Decoder.execute(to_be_finished);
        }
    }
}

void clock_control::flush() {
    ++clock;
    if (to_be_cleared) {
        int order[6];
        shuffle(order, 6);
        for (int i = 0; i < 6; ++i) {
            switch (order[i]) {
                case 1:
                    ROB.clear();
                    break;
                case 2:
                    RS.clear();
                    break;
                case 3:
                    LSB.clear();
                    break;
                case 4:
                    RF.clear();
                    break;
                case 5:
                    Decoder.clear();
                    break;
                case 6:
                    PC.clear();
            }
        }
        to_be_cleared = false;
    } else {
        int order[7];
        shuffle(order, 7);
        for (int i = 0; i < 7; ++i) {
            switch (order[i]) {
                case 1:
                    ROB.flush();
                    break;
                case 2:
                    RS.flush();
                    break;
                case 3:
                    LSB.flush();
                    break;
                case 4:
                    RF.flush();
                    break;
                case 5:
                    PC.flush();
                    break;
                case 6:
                    Decoder.flush();
                    break;
                case 7:
                    PRE.flush();
            }
        }
        if (to_be_finished) {
            if (LSB.empty()) { return_value = RF.get_return_value(); }
        }
    }
}

bool clock_control::finish() { return return_value != -1; }

int clock_control::get_return_value() { return return_value; }