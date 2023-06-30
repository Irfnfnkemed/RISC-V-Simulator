#include "reorder_buffer.h"
#include "reservation_staion.h"
#include "load_store_buffer.h"
#include "register_file.h"
#include "program_counter.h"
#include "decoder.h"
#include "predictor.h"

void reorder_buffer::set_tag(u_int8_t instr_, int tag_, u_int8_t dest_) {
    if (!((instr_ >= BEQ && instr_ <= BGEU) || (instr_ >= SB && instr_ <= SW))) {
        RF->set_tag(tag_, dest_);
    }
}

void reorder_buffer::get(u_int8_t reg_, int &depend_, int &value_) {
    RF->get(reg_, depend_, value_);
    if (depend_ != -1) {
        if (buffer.find(depend_).ready) {
            value_ = buffer.find(depend_).output;
            depend_ = -1;
        }
    }
}

void reorder_buffer::launch() {//发射指令
    if (!buffer.empty()) {
        reorder_buffer_unit &task = buffer_next.back();
        if (!task.launch) {
            task.launch = true;
            if (task.instr == END) {
                task.ready = true;//可以直接提交
            } else if (task.instr == LUI) {
                task.ready = true;//可以直接提交
            } else if (task.instr == AUIPC) {
                task.output = Address_ALU.execute(task.output, task.imd);//原本output存着PC
                task.ready = true;//可以提交
            } else if (task.instr == JAL) {
                task.output = Address_ALU.execute(task.output, 4);//原本output存着PC
                task.ready = true;//可以提交
            } else if (task.instr == JALR) {
                task.imd = Address_ALU.execute(task.imd, 4);//原本imd存着PC
                int depend, value;
                get(task.reg_one, depend, value);
                RS->add_instruction(JALR, value, -1, depend, -1, task.output, task.tag);
            } else if ((task.instr >= LB && task.instr <= SW)) {
                int depend, value;
                get(task.reg_one, depend, value);
                RS->add_instruction(task.instr, value, -1, depend, -1, task.imd, task.tag);
                LSB->add_instruction(task.tag);
            } else if ((task.instr >= LB && task.instr <= SRAI)) {//有立即数
                int depend, value;
                get(task.reg_one, depend, value);
                RS->add_instruction(task.instr, value, -1, depend, -1, task.imd, task.tag);
            } else {//两个寄存器
                int depend_one, value_one, depend_two, value_two;
                get(task.reg_one, depend_one, value_one);
                get(task.reg_two, depend_two, value_two);
                RS->add_instruction(task.instr, value_one, value_two, depend_one, depend_two, -1, task.tag);
            }
            set_tag(task.instr, task.tag, task.dest);//设置相关的依赖关系
        }
    }
}

void reorder_buffer::commit(bool &to_be_cleared) {
    if (!buffer.empty() && buffer.front().ready) {
        reorder_buffer_unit &task = buffer_next.front();
        if (task.instr >= SB && task.instr <= SW) {
            LSB->update_data(task.instr, task.output, RF->get(task.reg_two), task.tag);//返回到LSB中
        } else if (task.instr == JAL) {
            RF->flush_tag(task.tag, task.output, task.dest);//output暂存着PC+4
            if (task.dest > 0) { RS->flush_depend(task.tag, task.output); }//更新RS
        } else if (task.instr == JALR) {
            RF->flush_tag(task.tag, task.imd, task.dest);//imd暂存着PC+4
            if (task.dest > 0) { RS->flush_depend(task.tag, task.imd); }//更新RS
            PC->set_offset(task.output, true);
            PC->set_stop(false);//重新开始读入
        } else if (task.instr >= BEQ && task.instr <= BGEU) {
            if (task.output) {//跳转
                if (task.imd & 1) { PRE->change_counter(task.dest, true, true); }//预测正确
                else {
                    PRE->change_counter(task.dest, true, false);//预测错误
                    PC->set_offset(task.imd, true, true);//跳转PC
                    to_be_cleared = true;
                }
            } else {//不跳转
                if (task.imd & 1) {
                    PRE->change_counter(task.dest, false, false);//预测错误
                    PC->set_offset(task.imd + 3, true, true);//跳转PC
                    to_be_cleared = true;
                } else { PRE->change_counter(task.dest, false, true); }//预测正确
            }
        } else {
            RF->flush_tag(task.tag, task.output, task.dest);
            if (task.dest != 0) { RS->flush_depend(task.tag, task.output); }
        }
        buffer_next.pop();//删除队首
    }
}

void reorder_buffer::add_instruction() {
    if (!buffer.full() && !Decoder->is_send()) {
        u_int8_t instr, reg_one, reg_two, dest;
        int imd, output;
        Decoder->fetch_instr(instr, reg_one, reg_two, imd, dest, output);
        buffer_next.push(reorder_buffer_unit{buffer_next.get_index(), instr, reg_one, reg_two,
                                             imd, dest, output, false, false});
    }
    if (buffer.full()) {//已满，设置PC、Decoder一周期内不变
        PC->set_freeze();
        Decoder->set_freeze();
    }
}

void reorder_buffer::init(reservation_station *RS_, load_store_buffer *LSB_,
                          register_file *RF_, program_counter *PC_,
                          decoder *Decoder_, predictor *PRE_) {
    RS = RS_;
    LSB = LSB_;
    RF = RF_;
    PC = PC_;
    Decoder = Decoder_;
    PRE = PRE_;
}

void reorder_buffer::execute(bool &to_be_cleared) {
    launch();
    commit(to_be_cleared);
    add_instruction();
}

void reorder_buffer::set_ready(int tag_, int data_) {
    reorder_buffer_unit &task = buffer_next.find(tag_);
    task.output = data_;
    task.ready = true;
}

void reorder_buffer::flush() { buffer = buffer_next; }

void reorder_buffer::clear() {
    buffer.clear();
    buffer_next.clear();
}