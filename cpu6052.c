/*
 * Each Instruction has this format: OP Code (5 bits) - Argument 1 (27 bits)
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "cpu6052.h"

uint16_t addr_tmp = 0x0; // Help variable
uint16_t addr_rel = 0x0; // Help variable
uint8_t val_tmp = 0x0; // Help variable
operation cur_op; //Help variable

uint8_t read(uint16_t addr){
    //printf("Returning 0x%.2x from address 0x%.4x\n", mem[addr], addr);
    return mem[addr];
}

void write(uint16_t addr, uint8_t data){
    //printf("Returning 0x%.2x from address 0x%.4x\n", mem[addr], addr);
    mem[addr] = data;
}

void set_flag(uint16_t flag, uint8_t val){
    if(val > 0){
        st |= flag;
    }else{
        st &= ~flag;
    }
}

operation operations[] = {
        {"ADC", 0x69, ADC, IMM, 2},
        {"ADC", 0x65, ADC, ZPS, 3},
        {"ADC", 0x75, ADC, ZPX, 4},
        {"ADC", 0x6D, ADC, ABS, 4},
        {"ADC", 0x7D, ADC, ABX, 4},
        {"ADC", 0x79, ADC, ABY, 4},
        {"ADC", 0x61, ADC, IIR, 6},
        {"ADC", 0x71, ADC, IID, 5},
        {"AND", 0x29, AND, IMM, 2},
        {"AND", 0x25, AND, ZPS, 3},
        {"AND", 0x35, AND, ZPX, 4},
        {"AND", 0x2D, AND, ABS, 4},
        {"AND", 0x3D, AND, ABX, 4},
        {"AND", 0x39, AND, ABY, 4},
        {"AND", 0x21, AND, IIR, 6},
        {"AND", 0x31, AND, IID, 5},
        {"ASL", 0x0A, ASL, ACC, 2},
        {"ASL", 0x06, ASL, ZPS, 5},
        {"ASL", 0x16, ASL, ZPX, 6},
        {"ASL", 0x0E, ASL, ABS, 6},
        {"ASL", 0x1E, ASL, ABX, 7},
        {"BCC", 0x90, BCC, REL, 2},
        {"BCS", 0xB0, BCS, REL, 2},
        {"BEQ", 0xF0, BEQ, REL, 2},
        {"BIT", 0x24, BIT, ZPS, 3},
        {"BIT", 0x2c, BIT, ABS, 4},
        {"BMI", 0x30, BMI, REL, 2},
        {"BNE", 0xD0, BNE, REL, 2},
        {"BPL", 0x10, BPL, REL, 2},
        {"BPK", 0x00, BRK, IMP, 7},
        {"BVC", 0x50, BVC, REL, 2},
        {"BVS", 0x70, BVS, REL, 2},
        {"CLC", 0x18, CLC, IMP, 2},
        {"CLD", 0xD8, CLD, IMP, 2},
        {"CLI", 0x58, CLI, IMP, 2},
        {"CLV", 0xB8, CLV, IMP, 2},
        {"CMP", 0xC9, CMP, IMM, 2},
        {"CMP", 0xC5, CMP, ZPS, 3},
        {"CMP", 0xD5, CMP, ZPX, 4},
        {"CMP", 0xCD, CMP, ABS, 4},
        {"CMP", 0xDD, CMP, ABX, 4},
        {"CMP", 0xD9, CMP, ABY, 4},
        {"CMP", 0xC1, CMP, IIR, 6},
        {"CMP", 0xD1, CMP, IID, 5},
        {"CPX", 0xE0, CPX, IMM, 2},
        {"CPX", 0xE4, CPX, ZPS, 3},
        {"CPX", 0xEC, CPX, ABS, 4},
        {"CPY", 0xC0, CPY, IMM, 2},
        {"CPY", 0xC4, CPY, ZPS, 3},
        {"CPY", 0xCC, CPY, ABS, 4},
        {"DEC", 0xC6, DEC, ZPS, 5},
        {"DEC", 0xD6, DEC, ZPX, 6},
        {"DEC", 0xCE, DEC, ABS, 6},
        {"DEC", 0xDE, DEC, ABX, 7},
        {"DEX", 0xCA, DEX, IMP, 2},
        {"DEY", 0x88, DEY, IMP, 2},
        {"EOR", 0x49, EOR, IMM, 2},
        {"EOR", 0x45, EOR, ZPS, 3},
        {"EOR", 0x55, EOR, ZPX, 4},
        {"EOR", 0x4D, EOR, ABS, 4},
        {"EOR", 0x5D, EOR, ABX, 4},
        {"EOR", 0x59, EOR, ABY, 4},
        {"EOR", 0x41, EOR, IIR, 6},
        {"EOR", 0x51, EOR, IID, 5},
        {"INC", 0xE6, INC, ZPS, 5},
        {"INC", 0xF6, INC, ZPX, 6},
        {"INC", 0xEE, INC, ABS, 6},
        {"INC", 0xFE, INC, ABX, 7},
        {"INX", 0xE8, INX, IMP, 2},
        {"INY", 0xC8, INY, IMP, 2},
        {"JMP", 0x4C, JMP, ABS, 3},
        {"JMP", 0x6C, JMP, IDR, 5},
        {"JSR", 0x20, JSR, ABS, 6},
        {"LDA", 0xA9, LDA, IMM, 2},
        {"LDA", 0xA5, LDA, ZPS, 3},
        {"LDA", 0xB5, LDA, ZPX, 4},
        {"LDA", 0xAD, LDA, ABS, 4},
        {"LDA", 0xBD, LDA, ABX, 4},
        {"LDA", 0xB9, LDA, ABY, 4},
        {"LDA", 0xA1, LDA, IIR, 6},
        {"LDA", 0xB1, LDA, IID, 5},
        {"LDX", 0xA2, LDX, IMM, 2},
        {"LDX", 0xA6, LDX, ZPS, 3},
        {"LDX", 0xB6, LDX, ZPY, 4},
        {"LDX", 0xAE, LDX, ABS, 4},
        {"LDX", 0xBE, LDX, ABY, 4},
        {"LDY", 0xA0, LDY, IMM, 2},
        {"LDY", 0xA4, LDY, ZPS, 3},
        {"LDY", 0xB4, LDY, ZPX, 4},
        {"LDY", 0xAC, LDY, ABS, 4},
        {"LDY", 0xBC, LDY, ABX, 4},
        {"LSR", 0x4A, LSR, ACC, 2},
        {"LSR", 0x46, LSR, ZPS, 5},
        {"LSR", 0x56, LSR, ZPX, 6},
        {"LSR", 0x4E, LSR, ABS, 6},
        {"LSR", 0x5E, LSR, ABX, 7},
        {"NOP", 0xEA, NOP, IMP, 2},
        {"ORA", 0x09, ORA, IMM, 2},
        {"ORA", 0x05, ORA, ZPS, 3},
        {"ORA", 0x15, ORA, ZPX, 4},
        {"ORA", 0x0D, ORA, ABS, 4},
        {"ORA", 0x1D, ORA, ABX, 4},
        {"ORA", 0x19, ORA, ABY, 4},
        {"ORA", 0x01, ORA, IIR, 6},
        {"ORA", 0x11, ORA, IID, 5},
        {"PHA", 0x48, PHA, IMP, 3},
        {"PHP", 0x08, PHP, IMP, 3},
        {"PLA", 0x68, PLA, IMP, 4},
        {"PLP", 0x28, PLP, IMP, 4},
        {"ROL", 0x2A, ROL, ACC, 2},
        {"ROL", 0x26, ROL, ZPS, 5},
        {"ROL", 0x36, ROL, ZPX, 6},
        {"ROL", 0x2E, ROL, ABS, 6},
        {"ROL", 0x3E, ROL, ABX, 7},
        {"ROR", 0x6A, ROR, ACC, 2},
        {"ROR", 0x66, ROR, ZPS, 5},
        {"ROR", 0x76, ROR, ZPX, 6},
        {"ROR", 0x6E, ROR, ABS, 6},
        {"ROR", 0x7E, ROR, ABX, 7},
        {"RTI", 0x40, RTI, IMP, 6},
        {"RTS", 0x60, RTS, IMP, 6},
        {"SBC", 0xE9, SBC, IMM, 2},
        {"SBC", 0xE5, SBC, ZPS, 3},
        {"SBC", 0xF5, SBC, ZPX, 4},
        {"SBC", 0xED, SBC, ABS, 4},
        {"SBC", 0xFD, SBC, ABX, 4},
        {"SBC", 0xF9, SBC, ABY, 4},
        {"SBC", 0xE1, SBC, IIR, 6},
        {"SBC", 0xF1, SBC, IID, 5},
        {"SEC", 0x38, SEC, IMP, 2},
        {"SED", 0xF8, SED, IMP, 2},
        {"SEI", 0x78, SEI, IMP, 2},
        {"STA", 0x85, STA, ZPS, 3},
        {"STA", 0x95, STA, ZPX, 4},
        {"STA", 0x8D, STA, ABS, 4},
        {"STA", 0x9D, STA, ABX, 5},
        {"STA", 0x99, STA, ABY, 5},
        {"STA", 0x81, STA, IIR, 6},
        {"STA", 0x91, STA, IID, 6},
        {"STX", 0x86, STX, ZPS, 3},
        {"STX", 0x96, STX, ZPY, 4},
        {"STX", 0x8E, STX, ABS, 4},
        {"STY", 0x84, STY, ZPS, 3},
        {"STY", 0x94, STY, ZPX, 4},
        {"STY", 0x8C, STY, ABS, 4},
        {"TAX", 0xAA, TAX, IMP, 2},
        {"TAY", 0xA8, TAY, IMP, 2},
        {"TSX", 0xBA, TSX, IMP, 2},
        {"TXA", 0x8A, TXA, IMP, 2},
        {"TXS", 0x9A, TXS, IMP, 2},
        {"TYA", 0x98, TYA, IMP, 2},
};

operation get_operation(uint8_t op) {

    for (int i = 0; i < sizeof(operations)/sizeof(operation); i++) {
        if (operations[i].opcode == op) {
            return operations[i];
        }
    }

    fprintf(stderr, "OPCode %.2x is not implemented\n", op);
    return get_operation(0xEA);
    //exit(EXIT_FAILURE);
}

uint8_t IMP(){
    val_tmp = ac;
    return 0;
}

uint8_t ACC(){
    val_tmp = ac;
    return 0;
}

uint8_t IMM(){
    addr_tmp = pc++;
    val_tmp = read(addr_tmp);

    return 0;
}

uint8_t ZPS(){
    addr_tmp = read(pc++);
    val_tmp = read(addr_tmp);

    return 0;
}

uint8_t ZPX(){
    addr_tmp = read(pc++);
    addr_tmp += rx;
    addr_tmp &= 0xFF;
    val_tmp = read(addr_tmp);

    return 0;
}

uint8_t ZPY(){
    addr_tmp = read(pc++);
    addr_tmp += ry;
    addr_tmp &= 0xFF;
    val_tmp = read(addr_tmp);

    return 0;
}

uint8_t REL(){
    addr_rel = read(pc++);

    if(addr_rel & 0x80){
        addr_rel |= 0xFF00;
    }

    return 0;
}

uint8_t ABS(){
    uint8_t b1 = read(pc++);
    uint8_t b2 = read(pc++);
    addr_tmp = b1 | (b2 << 8);
    val_tmp = read(addr_tmp);

    return 0;
}

uint8_t ABX(){
    addr_tmp = pc++;
    uint8_t b1 = read(addr_tmp);
    addr_tmp = pc++;
    uint8_t b2 = read(addr_tmp);

    addr_tmp = (b1 | (b2 << 8)) + rx;

    val_tmp = read(addr_tmp);

    return b2 != (addr_tmp >> 8);
}

uint8_t ABY(){
    addr_tmp = pc++;
    uint8_t b1 = read(addr_tmp);
    addr_tmp = pc++;
    uint8_t b2 = read(addr_tmp);

    addr_tmp = b1 | (b2 << 8);
    addr_tmp += ry;

    val_tmp = read(addr_tmp);

    return b2 != (addr_tmp >> 8);
}

uint8_t IDR(){
    uint8_t pre1 = read(pc++);
    uint8_t pre2 = read(pc++);

    uint16_t pre_addr = pre1 | (pre2 << 8);

    if(pre1 == 0xFF){ //Page boundary hardwar bug
        addr_tmp = (read(pre_addr & 0xFF00) << 8) | read(pre_addr);
    }else{
        addr_tmp = (read(pre_addr + 1) << 8) | read(pre_addr);
    }

    return 0;
}

uint8_t IIR(){
    addr_tmp = read(pc++);
    uint8_t b1 = read((addr_tmp + rx) & 0xFF);
    uint8_t b2 = read((addr_tmp + rx + 1) & 0xFF);

    addr_tmp = b1 | (b2 << 8);
    val_tmp = read(addr_tmp);

    return 0;
}

uint8_t IID(){
    addr_tmp = read(pc++);
    uint8_t b1 = read(addr_tmp & 0xFF);
    uint8_t b2 = read(addr_tmp+1 & 0xFF);

    addr_tmp = b1 | (b2 << 8);
    addr_tmp += ry;

    val_tmp = read(addr_tmp);

    return b2 != (addr_tmp >> 8);
}

uint8_t LDA(){
    ac = val_tmp;

    set_flag(Z, ac == 0x00);
    set_flag(N, ac >> 7 == 1);

    printf("LDA Result: 0x%.2x\n", ac);

    return 1;
}

uint8_t LDX(){
    rx = val_tmp;

    set_flag(Z, rx == 0x00);
    set_flag(N, rx >> 7 == 1);

    printf("LDX Result: 0x%.2x\n", rx);

    return 1;
}

uint8_t LDY(){
    ry = val_tmp;

    set_flag(Z, ry == 0x00);
    set_flag(N, ry >> 7 == 1);

    printf("LDY Result: 0x%.2x\n", ry);

    return 1;
}

uint8_t STA(){
    write(addr_tmp, ac);

    printf("STA: Stored 0x%.2x to address 0x%.4x\n", ac, addr_tmp);
    return 0;
}

uint8_t STX(){
    write(addr_tmp, rx);

    printf("STX: Stored 0x%.2x to address 0x%.4x\n", rx, addr_tmp);

    return 0;
}

uint8_t STY(){
    write(addr_tmp, ry);

    printf("STY: Stored 0x%.2x to address 0x%.4x\n", ry, addr_tmp);

    return 0;
}

uint8_t TAX(){
    rx = ac;

    set_flag(Z, rx == 0x00);
    set_flag(N, rx >> 7 == 1);

    printf("TAX: X <- A (0x%.2x)\n", ac);

    return 0;
}

uint8_t TAY(){
    ry = ac;

    set_flag(Z, ry == 0x00);
    set_flag(N, ry >> 7 == 1);

    printf("TAY: Y <- A (0x%.2x)\n", ac);
    return 0;
}

uint8_t TXA(){
    ac = rx;

    set_flag(Z, ac == 0x00);
    set_flag(N, ac >> 7 == 1);

    printf("TXA: A <- X (0x%.2x)\n", rx);
    return 0;
}

uint8_t TYA(){
    ac = ry;

    set_flag(Z, ac == 0x00);
    set_flag(N, ac >> 7 == 1);

    printf("TYA: A <- Y (0x%.2x)\n", ry);
    return 0;
}

uint8_t TSX(){
    rx = sp;

    set_flag(Z, rx == 0x00);
    set_flag(N, rx >> 7 == 1);

    printf("TSX: X <- SP (0x%.2x)\n", sp);
    return 0;
}

uint8_t TXS(){
    sp = rx;
    return 0;
}

uint8_t PHA(){
    write(0x0100 + sp, ac);
    sp--;
    printf("PHA: (MEM <- AC) Stored 0x%.2x to address 0x%.4x\n", ac, 0x0101+sp);
    return 0;
}

uint8_t PHP(){
    write(0x0100 + sp, st);
    sp--;
    printf("PHP: (MEM <- ST) Stored 0x%.2x to address 0x%.4x\n", st, 0x0101+sp);

    return 0;
}

uint8_t PLA(){
    ac = read(0x0101 + sp);
    sp++;

    set_flag(Z, ac == 0x00);
    set_flag(N, ac >> 7 == 1);

    printf("PLA: (AC <- MEM) Fetched 0x%.2x from address 0x%.4x\n", ac, 0x0100+sp);

    return 0;
}

uint8_t PLP(){
    st = read(0x0101 + sp) | 0x20 ;
    sp++;

    printf("PLA: (ST <- MEM) Fetched 0x%.2x from address 0x%.4x\n", st, 0x0100+sp);

    return 0;
}

uint8_t AND(){
    printf("AND: 0x%.2x & 0x%.2x = ", ac, val_tmp);

    ac = val_tmp & ac;

    printf("0x%.2x\n", ac);

    set_flag(N, (ac & 0x80));
    set_flag(Z, ac == 0);

    return 1;
}

uint8_t EOR(){
    printf("EOR: 0x%.2x XOR 0x%.2x = ", ac, val_tmp);

    ac = val_tmp ^ ac;

    printf("0x%.2x\n", ac);

    set_flag(N, (ac & 0x80));
    set_flag(Z, ac == 0);

    return 1;
}

uint8_t ORA(){
    printf("ORA: 0x%.2x OR 0x%.2x = ", ac, val_tmp);

    ac = val_tmp | ac;

    printf("0x%.2x\n", ac);

    set_flag(N, (ac & 0x80));
    set_flag(Z, ac == 0);

    return 1;
}

uint8_t BIT(){
    set_flag(N, (val_tmp & 0x80));
    set_flag(V, (val_tmp & 0x40));
    set_flag(Z, (val_tmp & ac) == 0);

    printf("BIT: (0x%.2x & 0x%.2x = 0x%.2x) N: %i, V: %i, Z: %i\n", ac, val_tmp, (ac & val_tmp), (st & N) == N, (st & V) == V, (st & Z) == Z);

    return 0;
}

uint8_t ADC(){
    uint16_t result = (uint16_t)ac + (uint16_t)val_tmp + (uint16_t)(st & C);
    uint8_t carry_b = (st & C);
    set_flag(C, result & 0xFF00);
    set_flag(Z, (result & 0x00FF) == 0);
    set_flag(V, (result ^ (uint16_t)ac) & (result ^ val_tmp) & 0x0080);
    set_flag(N, result & 0x0080);

    if (st & D) {
        printf("Decimal Mode\n");
        set_flag(C, 0);

        uint8_t n = (ac & 0x0f) + (val_tmp & 0x0f);

        if (n >= 16) {
            result += 0x06;
        }
        if ((result & 0x0F) > 0x09) {
            printf("RES was 0x%.2x, now it's 0x%.2x\n", result, result + 0x06);
            result += 0x06;
        }
        if ((result & 0xF0) > 0x90) {
            printf("RES was 0x%.2x, now it's 0x%.2x\n", result, result + 0x60);

            result += 0x60;
            set_flag(C, 1);
        }

        set_flag(N, result & 0x0080);
        set_flag(Z, (result & 0xFF) == 0);
    }

    printf("ADC: AC (0x%.2x) + MEM (0x%.2x) + C (0x%.2x) = 0x%.2x\n", ac, val_tmp, carry_b, result);

    ac = result;

    return 1;
}

uint8_t SBC(){

    val_tmp ^= 0x00FF;
    uint16_t result = (uint16_t)ac + val_tmp + (uint16_t)(st & C);
    uint8_t carry_b = st & C;

    set_flag(C, (result & 0xFF00) > 0);
    set_flag(Z, (result & 0x00FF) == 0);
    set_flag(V, (result ^ (uint16_t)ac) & (result ^ val_tmp) & 0x0080);
    set_flag(N, result & 0x0080);

    if (st & D) {
        set_flag(C, 0);

        result -= 0x66;

        if ((result & 0x0F) > 0x09) {
            result += 0x06;
        }
        if ((result & 0xF0) > 0x90) {
            result += 0x60;
            set_flag(C, 1);
        }
    }

    printf("SBC: AC (0x%.2x) - MEM (0x%.2x) - ~C (0x%.2x) = 0x%.2x\n", ac, val_tmp, carry_b, result);

    ac = result;

    return 1;
}

uint8_t CMP(){
    uint8_t orig = val_tmp;
    val_tmp = ac - val_tmp;

    set_flag(N, val_tmp & 0x80);
    set_flag(Z, val_tmp == 0);
    set_flag(C, ac >= orig);

    printf("CMP: 0x%.2x - 0x%.2x, N: %i, Z: %i, C: %i\n", ac, orig, (st & N) == N, (st & Z) == Z, (st & C) == C);

    return 1;
}

uint8_t CPX(){
    uint8_t orig = val_tmp;
    val_tmp = rx - val_tmp;

    set_flag(N, val_tmp & 0x80);
    set_flag(Z, val_tmp == 0);
    set_flag(C, rx >= orig);

    printf("CPX: 0x%.2x - 0x%.2x, N: %i, Z: %i, C: %i\n", rx, orig, (st & N) == N, (st & Z) == Z, (st & C) == C);

    return 0;
}

uint8_t CPY(){
    uint8_t orig = val_tmp;
    val_tmp = ry - val_tmp;

    set_flag(N, val_tmp & 0x80);
    set_flag(Z, val_tmp == 0);
    set_flag(C, ry >= orig);

    printf("CPY: 0x%.2x - 0x%.2x, N: %i, Z: %i, C: %i\n", ry, orig, (st & N) == N, (st & Z) == Z, (st & C) == C);

    return 0;
}

uint8_t INC(){
    val_tmp++;

    set_flag(N, val_tmp & 0x80);
    set_flag(Z, val_tmp == 0);

    write(addr_tmp, val_tmp);
    printf("INC: [0x%.4x]: 0x%.2x + 1 = 0x%.2x\n", addr_tmp, val_tmp-1, val_tmp);
    return 0;
}

uint8_t INX(){
    rx++;

    set_flag(N, rx & 0x80);
    set_flag(Z, rx == 0);

    printf("INX: X: 0x%.2x + 1 = 0x%.2x\n", rx-1, rx);
    return 0;
}

uint8_t INY(){
    ry++;

    set_flag(N, ry & 0x80);
    set_flag(Z, ry == 0);

    printf("INX: X: 0x%.2x + 1 = 0x%.2x\n", ry-1, ry);
    return 0;
}

uint8_t DEC(){
    val_tmp--;

    set_flag(N, val_tmp & 0x80);
    set_flag(Z, val_tmp == 0);

    write(addr_tmp, val_tmp);
    printf("DEC: [0x%.4x]: 0x%.2x - 1 = 0x%.2x\n", addr_tmp, val_tmp+1, val_tmp);
    return 0;
}

uint8_t DEX(){
    rx--;

    set_flag(N, rx & 0x80);
    set_flag(Z, rx == 0);

    printf("DEX: X: 0x%.2x - 1 = 0x%.2x\n", rx+1, rx);
    return 0;
}

uint8_t DEY(){
    ry--;

    set_flag(N, ry & 0x80);
    set_flag(Z, ry == 0);

    printf("DEY: Y: 0x%.2x - 1 = 0x%.2x\n", ry+1, ry);
    return 0;
}

uint8_t ASL(){

    set_flag(C, val_tmp & 0x80);
    set_flag(N, val_tmp & 0x40);

    val_tmp = val_tmp << 1;

    set_flag(Z, val_tmp == 0);

    if(cur_op.addr_func == ACC){
        ac = val_tmp;
        printf("ASL: Shift Left (AC) = 0x%.2x\n", ac);
    }else{
        write(addr_tmp, val_tmp);
        printf("ASL: Shift Left (MEM 0x%.4x) = 0x%.2x\n", addr_tmp, val_tmp);
    }

    return 0;
}

uint8_t LSR(){

    set_flag(N, 0);
    set_flag(C, (val_tmp & 0x01));

    val_tmp = val_tmp >> 1;

    set_flag(Z, val_tmp == 0);

    if(cur_op.addr_func == ACC){
        ac = val_tmp;
        printf("LSR: Shift Right (AC) = 0x%.2x\n", ac);
    }else{
        write(addr_tmp, val_tmp);
        printf("LSR: Shift Right (MEM 0x%.4x) = 0x%.2x\n", addr_tmp, val_tmp);
    }

    return 0;
}

uint8_t ROL(){
    uint8_t carry = (st&C) == C;

    if(cur_op.addr_func == ACC){
        val_tmp = ac;
    }

    uint8_t val_shifted = (val_tmp << 1) + carry;

    set_flag(Z, val_shifted == 0x00);
    set_flag(N, (val_tmp & 0x40) == 0x40);
    set_flag(C, (val_tmp >> 7) == 1);

    if(cur_op.addr_func == ACC){
        ac = val_shifted;
        printf("ROL: A <- 0x%.2x\n", ac);
    }else{
        write(addr_tmp, val_shifted);
        printf("ROL: [0x%.4x] <- 0x%.2x\n", addr_tmp, val_shifted);
    }

    return 0;
}

uint8_t ROR(){
    uint8_t carry = (st&C) == 1;

    if(cur_op.addr_func == ACC){
        val_tmp = ac;
    }

    uint8_t val_shifted = (val_tmp >> 1) + (carry << 7);

    set_flag(Z, val_shifted == 0x00);
    set_flag(N, carry);
    set_flag(C, (val_tmp & 0x01) == 1);

    if(cur_op.addr_func == ACC){
        ac = val_shifted;
        printf("ROR: A <- 0x%.2x\n", ac);
    }else{
        write(addr_tmp, val_shifted);
        printf("ROR: [0x%.4x] <- 0x%.2x\n", addr_tmp, val_shifted);
    }

    return 0;
}

uint8_t JMP(){

    pc = addr_tmp;
    printf("JMP: Jump to 0x%.4x\n", pc);
    return 0;
}

uint8_t JSR(){

    pc--;
    write(0x0100 + sp--, (pc & 0xFF00) >> 8);
    write(0x0100 + sp--, pc & 0x00FF);

    pc = addr_tmp;

    printf("JSR: Jump to subroutine at 0x%.4x\n", pc);
    return 0;
}

uint8_t RTS(){

    uint8_t bit1 = read(0x0100 + ++sp);
    uint8_t bit2 = read(0x0100 + ++sp);

    pc = ((bit2 << 8) | bit1) + 1;

    printf("RTS: Return from subroutine to 0x%.4x\n", pc);
    return 0;
}

uint8_t BCC(){
    if((st & C) == 0){
        cycles++;
        addr_tmp = pc + addr_rel;

        //Check for page crossing
        if((addr_tmp & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addr_tmp;
        printf("BCC: (Overflow == 1) - Jumping to 0x%.4x\n", pc);
    }else{
        printf("BCC: (Overflow == 1) - No Jump\n");
    }

    return 0;
}

uint8_t BCS(){

    if((st & C) == 1){
        cycles++;
        addr_tmp = pc + addr_rel;

        //Check for page crossing
        if((addr_tmp & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addr_tmp;
        printf("BCS: (Carry == 1) - Jumping to 0x%.4x\n", pc);
    }else{
        printf("BCS: (Carry == 1) - No Jump\n");
    }

    return 0;
}

uint8_t BEQ(){
    if((st & Z) == 1){
        cycles++;
        addr_tmp = pc + addr_rel;

        //Check for page crossing
        if((addr_tmp & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addr_tmp;
        printf("BEQ: (Zero == 1) - Jumping to 0x%.4x\n", pc);
    }else{
        printf("BEQ: (Zero == 1) - No Jump\n");
    }

    return 0;
}

uint8_t BMI(){
    if((st & N) == 0){
        cycles++;
        addr_tmp = pc + addr_rel;

        //Check for page crossing
        if((addr_tmp & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addr_tmp;
        printf("BMI: (Negative == 1) - Jumping to 0x%.4x\n", pc);
    }else{
        printf("BMI: (Negative == 1) - No Jump\n");
    }

    return 0;
}

uint8_t BNE(){

    if((st & Z) == 0){
        cycles++;
        addr_tmp = pc + addr_rel;

        //Check for page crossing
        if((addr_tmp & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addr_tmp;
        printf("BNE: (Zero == 1) - Jumping to 0x%.4x\n", pc);
    }else{
        printf("BNE: (Zero == 1) - No Jump\n");
    }

    return 0;
}

uint8_t BPL(){

    if((st & N) == 0){
        cycles++;
        addr_tmp = pc + addr_rel;

        //Check for page crossing
        if((addr_tmp & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addr_tmp;
        printf("BPL: (Negative == 0) - Jumping to 0x%.4x\n", pc);
    }else{
        printf("BPL: (Negative == 0) - No Jump\n");
    }

    return 0;
}

uint8_t BVC(){

    if((st & V) == 0){
        cycles++;
        addr_tmp = pc + addr_rel;

        //Check for page crossing
        if((addr_tmp & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addr_tmp;
        printf("BVC: (Overflow == 0) - Jumping to 0x%.4x\n", pc);
    }else{
        printf("BVC: (Overflow == 0) - No Jump\n");
    }

    return 0;
}

uint8_t BVS(){

    if((st & V) == 1){
        cycles++;
        addr_tmp = pc + addr_rel;

        //Check for page crossing
        if((addr_tmp & 0xFF00) != (pc & 0xFF00)){
            cycles++;
        }

        pc = addr_tmp;
        printf("BVS: (Overflow == 1) - Jumping to 0x%.4x\n", pc);
    }else{
        printf("BVS: (Overflow == 1) - No Jump\n");
    }

    return 0;
}

uint8_t CLC(){
    set_flag(C, 0);
    printf("CLC: Reset Carry flag\n");
    return 0;
}

uint8_t CLD(){
    set_flag(D, 0);
    printf("CLD: Reset Decimal flag\n");
    return 0;
}

uint8_t CLI(){
    set_flag(I, 0);
    printf("CLI: Reset Interrupt flag\n");
    return 0;
}

uint8_t CLV(){
    set_flag(V, 0);
    printf("CLV: Set Overflow flag\n");
    return 0;
}

uint8_t SEC(){
    set_flag(C, 1);
    printf("SEC: Set Carry flag\n");
    return 0;
}

uint8_t SED(){
    set_flag(D, 1);
    printf("SED: Set Decimal flag\n");
    return 0;
}

uint8_t SEI(){
    set_flag(I, 1);
    printf("SEI: Set Interrupt flag\n");
    return 0;
}

uint8_t BRK(){

    pc++;

    write(0x0100 + sp--, pc & 0x00FF);
    write(0x0100 + sp--, pc >> 8);
    write(0x0100 + sp--, st | B);

    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);

    printf("BRK: Pushed PC (0x%.4x) and ST (0x%.2x) to stack\n", pc, st);

    set_flag(I, 1);

    return 0;
}

uint8_t NOP(){

    switch (opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
        default:
            return 0;
    }

}

uint8_t RTI(){
    st = read(0x0100 + ++sp);//Read status
    st &= ~B; //Set Break Bit to 0
    st |= U; //Set unused bit to 1

    uint8_t bit1 = read(0x0100 + ++sp);
    uint8_t bit2 = read(0x0100 + ++sp);

    addr_tmp = bit1 | (bit2 << 8);
    pc = addr_tmp;

    printf("RTI: New PC: 0x%.4x, New Status: 0x%.2x\n", pc, st);

    return 0;
}

int clock(){
    if(cycles == 0){
        opcode = read(pc++); //Read OP Code and increase Program Counter
        cur_op = get_operation(opcode);
        if(opcode == 0)return 0;
        printf("Executing: %s\n", cur_op.name);
        cycles = cur_op.cycles;
        cur_op.addr_func();
        cur_op.instr_func();

    }
    cycles--;
    return 1;
}

void run(){
    while(clock());

}

void reset(uint16_t start_address){

    pc = start_address;
    sp = 0xFF;
    ac = 0;
    rx = 0;
    ry = 0;
    st = U;

    cycles = 8;
}

void print_memory() {

    printf("\n===== Memory View =====\n");
    int j = 0;

    for (int i = 0; i < 65536; i++) {
        if (mem[i] != 0) {
            printf("[%.4x]: %.2x\n", i, mem[i]);
            j++;
        }
    }
    printf("%i non-zero value(s)\n\n", j);
}

  int main(int argc, char** argv){
    memset(mem, 0, sizeof(mem));

    unsigned char c[] = {

            0x20, 0x54, 0x06, 0x20, 0x70, 0x06, 0x20, 0xc9, 0x06, 0x4c, 0x03, 0x06, 0x60, 0x48, 0x8a, 0x48,
            0xa9, 0x00, 0xa6, 0x10, 0x9d, 0x00, 0x05, 0xa6, 0x78, 0xa9, 0x01, 0x9d, 0x00, 0x05, 0x86, 0x10,
            0xa9, 0x00, 0xa6, 0x11, 0x9d, 0x00, 0x05, 0xa6, 0x79, 0xa9, 0x03, 0x9d, 0x00, 0x05, 0x86, 0x11,
            0xa9, 0x00, 0xa6, 0x12, 0x9d, 0x00, 0x05, 0xa6, 0x7a, 0xa9, 0x04, 0x9d, 0x00, 0x05, 0x86, 0x12,
            0xa9, 0x00, 0xa6, 0x13, 0x9d, 0x00, 0x05, 0xa6, 0x7b, 0xa9, 0x04, 0x9d, 0x00, 0x05, 0x86, 0x13,
            0x68, 0xaa, 0x68, 0x60, 0xa2, 0x00, 0xad, 0x0a, 0x07, 0x9d, 0x00, 0x02, 0x9d, 0x00, 0x04, 0xca,
            0xe0, 0x00, 0xd0, 0xf5, 0xa9, 0x10, 0x85, 0x80, 0xa2, 0x0f, 0x95, 0x81, 0xca, 0x10, 0xfb, 0x60,
            0xa9, 0x00, 0x85, 0x78, 0xa9, 0x20, 0x85, 0x79, 0xa9, 0xc0, 0x85, 0x7a, 0xa9, 0xe0, 0x85, 0x7b,
            0xa2, 0x0f, 0xb5, 0x81, 0x95, 0x82, 0xa8, 0x84, 0x02, 0xb9, 0xea, 0x06, 0x85, 0x00, 0xc8, 0xb9,
            0xea, 0x06, 0x85, 0x01, 0xad, 0x0a, 0x07, 0xa4, 0x78, 0x91, 0x00, 0xc8, 0x91, 0x00, 0xa4, 0x7b,
            0x91, 0x00, 0xc8, 0x91, 0x00, 0xa4, 0x79, 0xa9, 0x00, 0x91, 0x00, 0xc8, 0x91, 0x00, 0xa4, 0x7a,
            0x91, 0x00, 0xc8, 0x91, 0x00, 0xe6, 0x78, 0xe6, 0x79, 0xe6, 0x7a, 0xe6, 0x7b, 0xe6, 0x78, 0xe6,
            0x79, 0xe6, 0x7a, 0xe6, 0x7b, 0xca, 0x10, 0xba, 0x60, 0xa5, 0x80, 0xc5, 0x81, 0xf0, 0x09, 0xa5,
            0x80, 0x18, 0xe5, 0x81, 0x10, 0x0f, 0x30, 0x08, 0xa5, 0xfe, 0x29, 0x0f, 0x0a, 0x85, 0x80, 0x60,
            0xc6, 0x81, 0xc6, 0x81, 0x60, 0xe6, 0x81, 0xe6, 0x81, 0x60, 0x00, 0x02, 0x20, 0x02, 0x40, 0x02,
            0x60, 0x02, 0x80, 0x02, 0xa0, 0x02, 0xc0, 0x02, 0xe0, 0x02, 0x00, 0x03, 0x20, 0x03, 0x40, 0x03,
            0x60, 0x03, 0x80, 0x03, 0xa0, 0x03, 0xc0, 0x03, 0xe0, 0x03, 0x0d

    };

    for(int i = 0; i < sizeof(c); i++){
        mem[0x0600 + i] = c[i];
    }

    reset(0x0600);
    run();
    print_memory();

    return EXIT_SUCCESS;
}