#include "decoder.h"
#include "program_counter.h"
#include "reorder_buffer.h"
#include "predictor.h"

void decoder::init(program_counter *PC_, memory *MEM_, predictor *PRE_) {
    PC = PC_;
    MEM = MEM_;
    PRE = PRE_;
}

void decoder::decode(int instr_bin, decode_instr &out, bool &to_be_finished) {
    if (instr_bin == 0x0ff00513) {
        out.instr = 0xff;
        PC->set_stop(true);
        to_be_finished = true;
        return;
    }
    switch (fetch(instr_bin, 6, 0)) {
        case 0b0110111:
            out.instr = LUI;
            out.other = fetch(instr_bin, 31, 12) << 12;
            out.dest = fetch(instr_bin, 11, 7);
            return;
        case 0b0010111:
            out.instr = AUIPC;
            out.other = PC->get_pc();
            out.imd = fetch(instr_bin, 31, 12) << 12;
            out.dest = fetch(instr_bin, 11, 7);
            return;
        case 0b1101111:
            out.instr = JAL;
            out.other = PC->get_pc();
            out.imd = sign_extend((fetch(instr_bin, 30, 21) << 1) + (((instr_bin >> 20) & 1) << 11) +
                                  (fetch(instr_bin, 19, 12) << 12) + (((instr_bin >> 31) & 1) << 20), 21);
            out.dest = fetch(instr_bin, 11, 7);
            PC->set_offset(out.imd);//跳转PC
            return;
        case 0b1100111:
            out.instr = JALR;
            out.reg_one = fetch(instr_bin, 19, 15);
            out.imd = PC->get_pc();//此处与其他指令不同，imd存PC，other存立即数
            out.other = sign_extend(fetch(instr_bin, 31, 20), 12);
            out.dest = fetch(instr_bin, 11, 7);
            PC->set_stop(true);
            return;
        case 0b1100011:
            out.reg_one = fetch(instr_bin, 19, 15);
            out.reg_two = fetch(instr_bin, 24, 20);
            //分支指令，若预测跳转，imd置为现PC+1(offset为2的倍数)，跳转PC；反之，imd置为PC+offset
            out.imd = sign_extend((fetch(instr_bin, 11, 8) << 1) + (fetch(instr_bin, 30, 25) << 5) +
                                  (((instr_bin >> 7) & 1) << 11) + (((instr_bin >> 31) & 1) << 12), 13);
            out.dest = (PC->get_pc()) >> 2;//dest存PC/4，用于寻找对应的分支预测器
            if (PRE->jump(out.dest)) {
                PC->set_offset(out.imd);
                out.imd = PC->get_pc() + 1;
            } else { out.imd += PC->get_pc(); }
            switch (fetch(instr_bin, 14, 12)) {
                case 0b000:
                    out.instr = BEQ;
                    return;
                case 0b001:
                    out.instr = BNE;
                    return;
                case 0b100:
                    out.instr = BLT;
                    return;
                case 0b101:
                    out.instr = BGE;
                    return;
                case 0b110:
                    out.instr = BLTU;
                    return;
                case 0b111:
                    out.instr = BGEU;
                    return;
            }
        case 0b0000011:
            out.reg_one = fetch(instr_bin, 19, 15);
            out.imd = sign_extend(fetch(instr_bin, 31, 20), 12);
            out.dest = fetch(instr_bin, 11, 7);
            switch (fetch(instr_bin, 14, 12)) {
                case 0b000:
                    out.instr = LB;
                    return;
                case 0b001:
                    out.instr = LH;
                    return;
                case 0b010:
                    out.instr = LW;
                    return;
                case 0b100:
                    out.instr = LBU;
                    return;
                case 0b101:
                    out.instr = LHU;
                    return;
            }
        case 0b0100011:
            out.reg_one = fetch(instr_bin, 19, 15);
            out.reg_two = fetch(instr_bin, 24, 20);
            out.imd = sign_extend(fetch(instr_bin, 11, 7) + (fetch(instr_bin, 31, 25) << 5), 12);
            switch (fetch(instr_bin, 14, 12)) {
                case 0b000:
                    out.instr = SB;
                    return;
                case 0b001:
                    out.instr = SH;
                    return;
                case 0b010:
                    out.instr = SW;
                    return;
            }
        case 0b0010011:
            out.reg_one = fetch(instr_bin, 19, 15);
            out.imd = sign_extend(fetch(instr_bin, 31, 20), 12);
            out.dest = fetch(instr_bin, 11, 7);
            switch (fetch(instr_bin, 14, 12)) {
                case 0b000:
                    out.instr = ADDI;
                    return;
                case 0b010:
                    out.instr = SLTI;
                    return;
                case 0b011:
                    out.instr = SLTIU;
                    return;
                case 0b100:
                    out.instr = XORI;
                    return;
                case 0b110:
                    out.instr = ORI;
                    return;
                case 0b111:
                    out.instr = ANDI;
                    return;
                case 0b001:
                    out.instr = SLLI;
                    return;
                case 0b101:
                    if ((instr_bin >> 30) & 1) { out.instr = SRAI; }
                    else { out.instr = SRLI; }
                    return;
            }
        case 0b0110011:
            out.reg_one = fetch(instr_bin, 19, 15);
            out.reg_two = fetch(instr_bin, 24, 20);
            out.dest = fetch(instr_bin, 11, 7);
            switch (fetch(instr_bin, 14, 12)) {
                case 0b000:
                    if ((instr_bin >> 30) & 1) { out.instr = SUB; }
                    else { out.instr = ADD; }
                    return;
                case 0b001:
                    out.instr = SLL;
                    return;
                case 0b010:
                    out.instr = SLT;
                    return;
                case 0b011:
                    out.instr = SLTU;
                    return;
                case 0b100:
                    out.instr = XOR;
                    return;
                case 0b101:
                    if ((instr_bin >> 30) & 1) { out.instr = SRA; }
                    else { out.instr = SRL; }
                    return;
                case 0b110:
                    out.instr = OR;
                    return;
                case 0b111:
                    out.instr = AND;
                    return;
            }
    }
}

bool decoder::is_send() const { return instr_decode.instr == 0xff; }

void decoder::execute(bool &to_be_finished) {
    if (!PC->is_stop()) {//读指令
        decode(MEM->load_memory((PC->get_pc()), 4), instr_decode_next, to_be_finished);
    }
}

void decoder::fetch_instr(u_int8_t &instr_, u_int8_t &reg_one_, u_int8_t &reg_two_,
                          int &imd_, u_int8_t &dest_, int &other) {
    instr_ = instr_decode.instr;
    reg_one_ = instr_decode.reg_one;
    reg_two_ = instr_decode.reg_two;
    imd_ = instr_decode.imd;
    dest_ = instr_decode.dest;
    other = instr_decode.other;
    instr_decode.instr = 0xff;
}

void decoder::set_freeze() { freeze = true; }

void decoder::flush() {
    if (freeze) { freeze = false; }
    else {
        instr_decode = instr_decode_next;
        instr_decode_next.instr = 0xff;
    }
}

void decoder::clear() {
    instr_decode.instr = instr_decode_next.instr = 0xff;
    freeze = false;
}