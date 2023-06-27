#ifndef RISC_V_SIMULATOR_TOOL_H
#define RISC_V_SIMULATOR_TOOL_H

#include <cstring>

enum INSTRUCTION {
    LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU, BGEU,
    LB, LH, LW, LBU, LHU, SB, SH, SW,
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND, END
};

//将有digit位(1-based)的16进制数转换为10进制数
int hex_to_dec(const char source[], int digit);

//将有digit位(1-based)(二进制下)的10进制数符号扩展为32位
int sign_extend(int source, int digit);

//得到指令的opcode(low--high位，0-based)，并转为十进制
int fetch(int source, int high, int low);

//可容纳size-1个元素的循环队列
//为了取模方便，size应为2的次方
template<class element, int size>
class queue {
private:
    element Queue[size];
    int head = 0, tail = 0;//循环队列的头、尾
    const int mod = size - 1;
    int base = 0;//用于计算基于下标的tag

public:
    class iterator {
    private:
        friend class queue<element, size>;

        queue *to_queue;
        int point;
        const int mod = size - 1;

    public:
        iterator(queue *to_queue_, int point_) {
            to_queue = to_queue_;
            point = point_;
        }

        iterator &operator++() {
            point = (++point) & mod;
            return *this;
        }

        bool operator!=(const iterator &obj) { return point != obj.point; }

        element *operator->() { return &(to_queue->Queue[point]); }

        element &operator*() { return to_queue->Queue[point]; }
    };

    queue<element, size> &operator=(const queue<element, size> &obj) {
        if (&obj == this) { return *this; }
        for (int i = obj.head; i != obj.tail; i = (++i) & mod) { Queue[i] = obj.Queue[i]; }
        head = obj.head;
        tail = obj.tail;
        return *this;
    }

    bool empty() { return head == tail; }

    bool full() { return ((tail + 1) & mod) == head; }

    void push(const element &new_element) {
        Queue[tail] = new_element;
        tail = (tail + 1) & mod;
        if (tail == 0) { ++base; }
    }

    element pop() {
        int pos = head;
        head = (head + 1) & mod;
        return Queue[pos];
    }

    element &front() { return Queue[head]; }

    element &back() { return Queue[(tail - 1 + size) & mod]; }

    iterator begin() { return iterator(this, head); }

    iterator end() { return iterator(this, tail); }

    //给出新加入队尾下标对应的tag
    int get_index() { return tail + base * size; }

    //按下标寻找
    element &find(int index) { return Queue[index]; }

    void clear() { head = tail = 0; }
};


#endif //RISC_V_SIMULATOR_TOOL_H
