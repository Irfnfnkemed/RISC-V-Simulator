#include "clock_control.h"

int main() {
    //freopen("aaa", "r", stdin);
    //freopen("sss", "w", stdout);
    clock_control a;
    a.init();
    while (a.finish() == -1) {
//        std::cout << "-----------------\n" << a.clock << ":\n";
        a.execute();
        a.flush();
    }
    std::cout << std::dec << a.finish() << std::endl;
    return 0;
}