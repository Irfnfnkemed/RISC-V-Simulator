#ifndef RISC_V_SIMULATOR_TOOL_H
#define RISC_V_SIMULATOR_TOOL_H

#include <cstring>

//将有digit位的16进制数转换为10进制数
int hex_to_dec(const char source[], int digit);

//将10进制数转换为digit位的2进制数
void dec_to_bin(bool target[], int source, int digit);

//将有digit位的2进制数转换为10进制数
int bin_to_dec(const bool source[], int digit);

//将有digit位(二进制下)的10进制数符号扩展为32位
int sign_extend(int source, int digit);

//可容纳size-1个元素的循环队列
//为了取模方便，size应为2的次方
template<class element, int size>
class queue {
private:
    element Queue[size];
    int head = 0, tail = 0;//循环队列的头、尾
    const int mod = size - 1;
public:
    bool empty() { return head == tail; }

    bool full() { return ((tail + 1) & mod) == head; }

    void push(const element &new_element) {
        Queue[tail] = new_element;
        tail = (tail + 1) & mod;
    }

    element pop() {
        int pos = head;
        head = (head + 1) & mod;
        return Queue[pos];
    }

    element &front() { return Queue[head]; }
};


#endif //RISC_V_SIMULATOR_TOOL_H
