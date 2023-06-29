#include "reorder_buffer.h"
#include "reservation_staion.h"
#include "load_store_buffer.h"
#include "register_file.h"
#include "program_counter.h"
#include "decoder.h"
#include "predictor.h"

void reorder_buffer::set_tag(int instr_, int tag_, int dest_) {
    if (!((instr_ >= BEQ && instr_ <= BGEU) || (instr_ >= SB && instr_ <= SW))) {
        RF->set_tag(tag_, dest_);
    }
}

void reorder_buffer::get(int reg_, int &depend_, int &value_) {
    RF->get(reg_, depend_, value_);
    if (depend_ != -1) {
        if (buffer.find(depend_).ready) {
            value_ = buffer.find(depend_).imd;
            depend_ = -1;
        }
    }
}

void reorder_buffer::launch() {//发射指令
    if (!buffer.empty()) {
        reorder_buffer_unit &task = buffer_next.back();
        if (!task.launch) {
//            std::cout << "launch ";
//            aaa(task.instr);
            task.launch = true;
            if (task.instr == END) {
                task.ready = true;//可以直接提交
            } else if (task.instr == LUI) {
                task.ready = true;//可以直接提交
            } else if (task.instr == AUIPC) {
                task.imd = Address_ALU.execute(task.reg_two, task.imd);//原本reg_two存着PC
                task.ready = true;//可以提交
            } else if (task.instr == JAL) {
                task.imd = Address_ALU.execute(task.reg_two, 4);//原本reg_two存着PC
                task.ready = true;//可以提交
            } else if (task.instr == JALR) {
                task.reg_two = Address_ALU.execute(task.reg_two, 4);//用reg_two暂存PC+4
                int depend, value;
                get(task.reg_one, depend, value);
                RS->add_instruction(JALR, value, -1, depend, -1, task.imd, task.tag);
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

void reorder_buffer::commit(bool &to_be_cleared, bool &a) {
    if (!buffer.empty() && buffer.front().ready) {
        reorder_buffer_unit &task = buffer_next.front();
//        std::cout << "commit pc= " << std::hex << task.pc << std::dec << '\n';
//        a = true;
        ////aaa(task.instr);
        if (task.instr >= SB && task.instr <= SW) {
            LSB->update_data(task.instr, task.imd, RF->get(task.reg_two), task.tag);//返回到LSB中
        } else if (task.instr == JAL) {
            RF->flush_tag(task.tag, task.imd, task.dest);//imd暂存着PC+4
            if (task.dest > 0) { RS->flush_depend(task.tag, task.reg_two); }//更新RS
        } else if (task.instr == JALR) {
            RF->flush_tag(task.tag, task.reg_two, task.dest);//reg_two暂存着PC+4
            if (task.dest > 0) { RS->flush_depend(task.tag, task.reg_two); }//更新RS
            PC->set_offset(task.imd, true);
            PC->set_stop(false);//重新开始读入
        } else if (task.instr >= BEQ && task.instr <= BGEU) {
            if (task.imd) {//跳转
                if (task.dest & 1) { PRE->change_counter(true); }//预测正确
                else {
                    PRE->change_counter(false);//预测错误
                    PC->set_offset(task.dest, true, true);//跳转PC
                    to_be_cleared = true;
                }
            } else {//不跳转
                if (task.dest & 1) {
                    PRE->change_counter(false);//预测错误
                    PC->set_offset(task.dest + 3, true, true);//跳转PC
                    to_be_cleared = true;
                } else { PRE->change_counter(true); }//预测正确
            }
        } else {
            RF->flush_tag(task.tag, task.imd, task.dest);
            if (task.dest != 0) { RS->flush_depend(task.tag, task.imd); }
        }
        buffer_next.pop();//删除队首
    }
}

void reorder_buffer::add_instruction() {
    if (!buffer.full() && !Decoder->is_send()) {
        int instr, reg_one, reg_two, imd, dest, pc;
        Decoder->fetch_instr(instr, reg_one, reg_two, imd, dest, pc);
        buffer_next.push(reorder_buffer_unit{buffer_next.get_index(), instr, reg_one, reg_two,
                                             imd, dest, false, false, pc});
    }
    if (buffer.full()) {
        PC->ppp();
        Decoder->ppp();
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

void reorder_buffer::execute(bool &to_be_cleared, bool &a) {
    launch();
    commit(to_be_cleared, a);
    add_instruction();
}

void reorder_buffer::set_ready(int tag_, int data_) {
    reorder_buffer_unit &task = buffer_next.find(tag_);
    task.imd = data_;
    task.ready = true;
}

void reorder_buffer::flush() { buffer = buffer_next; }

void reorder_buffer::clear() {
    buffer.clear();
    buffer_next.clear();
}