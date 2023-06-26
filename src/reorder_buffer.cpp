#include "reorder_buffer.h"
#include "reservation_staion.h"
#include "load_store_buffer.h"
#include "register_file.h"
#include "program_counter.h"

void reorder_buffer::set_tag(int instr_, int tag_, int dest_) {
    if (!((instr_ >= BEQ && instr_ <= BGEU) || (instr_ >= SB && instr_ <= SW))) {
        RF->set_tag(tag_, dest_);
    }
}

void reorder_buffer::launch() {//发射指令
    reorder_buffer_unit &task = buffer_next.back();
    if (!buffer.empty() && !task.launch) {
        task.launch = true;
        if (task.instr == LUI) {
            task.ready = true;//可以直接提交
        } else if (task.instr == AUIPC) {
            task.imd = Address_ALU.execute(task.reg_two, task.imd);//原本reg_two存着PC
            task.ready = true;//可以提交
        } else if (task.instr == JAL) {
            task.reg_two = Address_ALU.execute(task.reg_two, 4);//用reg_two暂存PC+4
            task.ready = true;//可以提交
        } else if (task.instr == JALR) {
            task.reg_two = Address_ALU.execute(task.reg_two, 4);//用reg_two暂存PC+4
            RS->add_instruction(JALR, RF->get_value(task.reg_one), -1,
                                RF->get_depend(task.reg_one), -1, task.imd, task.tag);
        } else if ((task.instr >= LB && task.instr <= SW)) {
            RS->add_instruction(task.instr, RF->get_value(task.reg_one), -1,
                                RF->get_depend(task.reg_one), -1, task.imd, task.tag);
            LSB->add_instruction(task.tag);
        } else if ((task.instr >= LB && task.instr <= SRAI)) {//有立即数
            RS->add_instruction(task.instr, RF->get_value(task.reg_one), -1,
                                RF->get_depend(task.reg_one), -1, task.imd, task.tag);
        } else {//两个寄存器
            RS->add_instruction(task.instr, RF->get_value(task.reg_one), RF->get_value(task.reg_two),
                                RF->get_depend(task.reg_one), RF->get_depend(task.reg_two), -1, task.tag);
        }
        set_tag(task.instr, task.tag, task.dest);//设置相关的依赖关系
    }
}

void reorder_buffer::commit() {
    if (!buffer.empty() && buffer.front().ready) {
        reorder_buffer_unit &task = buffer_next.front();
        if (task.instr >= SB && task.instr <= SW) {
            LSB->update_data(task.instr, task.imd, RF->get_value(task.reg_two), -1);//返回到LSB中
        } else if (task.instr == JAL) {
            RF->flush_tag(task.tag, task.reg_two);//reg_two暂存着PC+4
        } else if (task.instr == JALR) {
            RF->flush_tag(task.tag, task.reg_two);//reg_two暂存着PC+4
            PC->set_offset(task.imd);
            ////////////////////////////////////////stop = true;
        } else if (task.instr >= BEQ && task.instr <= BGEU) {
            ///////////////////////////////////////////if (task.imd) { PC->set_offset(task.dest); }//分支指令，将偏移量暂存在dest中
        } else { RF->flush_tag(task.tag, task.imd); }
        buffer_next.pop();//删除队首
    }
}

void reorder_buffer::init(reservation_station *RS_, load_store_buffer *LSB_,
                          register_file *RF_, program_counter *PC_) {
    RS = RS_;
    LSB = LSB_;
    RF = RF_;
    PC = PC_;
}

void reorder_buffer::execute() {
    launch();
    commit();
//////////////////////////////
}

void reorder_buffer::set_ready(int tag_, int data_) {
    for (auto iter = buffer_next.begin(); iter != buffer_next.end(); ++iter) {
        if (iter->tag == tag_) {
            iter->imd = data_;
            iter->ready = true;
        }
    }
}

void reorder_buffer::flush() { buffer = buffer_next; }