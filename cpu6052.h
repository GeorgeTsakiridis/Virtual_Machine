//
// Created by george on 1/10/21.
//

#ifndef VIRTUAL_MACHINE_CPU6052_H
#define VIRTUAL_MACHINE_CPU6052_H

#include <stdint.h>

enum FLAGS{
    C = 1 << 0, //Carry Flag
    Z = 1 << 1, //Zero Flag
    I = 1 << 2, //Interrupt Flag
    D = 1 << 3, //Decimal Flag
    B = 1 << 4, //Break Flag
    U = 1 << 5, //Unused Flag
    V = 1 << 6, //Overflow Flag
    N = 1 << 7 //Negative Flag
};

uint16_t pc = 0x00; //Program Counter
uint8_t sp = 0x0; //Stack Pointer
uint8_t ac = 0x0; //Accumulator
uint8_t rx = 0x0; //Register X
uint8_t ry = 0x0; //Register Y
uint8_t st = U; //Status Register

uint8_t cycles = 0; //Cycles left for instruction to complete
u_int8_t opcode = 0; //Current op code

uint8_t mem[65536]; //64kb of RAM

typedef struct operation {
    char* name;
    uint8_t opcode;
    uint8_t (*instr_func)();
    uint8_t (*addr_func)();
    uint8_t cycles;
} operation;

//Addressing modes
uint8_t IMP(); //Implicit
uint8_t ACC(); //Accumulator
uint8_t IMM(); //Immediate
uint8_t ZPS(); //Zero Page
uint8_t ZPX(); //Zero Page X
uint8_t ZPY(); //Zero Page Y
uint8_t REL(); //Relative
uint8_t ABS(); //Absolute
uint8_t ABX(); //Absolute X
uint8_t ABY(); //Absolute Y
uint8_t IDR(); //Indirect
uint8_t IIR(); //Indexed Indirect X
uint8_t IID(); //Inderect Indexed Y

//Instructions
uint8_t LDA(); //Load Accumulator
uint8_t LDX(); //Load X Register
uint8_t LDY(); //Load Y Register
uint8_t STA(); //Store Accumulator
uint8_t STX(); //Store X Register
uint8_t STY(); //Store Y Register
uint8_t TAX(); //Transfer Accumulator to X
uint8_t TAY(); //Transfer Accumulator to Y
uint8_t TXA(); //Transfer X to Accumulator
uint8_t TYA(); //Transfer Y to Accumulator
uint8_t TSX(); //Transfer stack pointer to X
uint8_t TXS(); //Transfer X to stack pointer
uint8_t PHA(); //Push accumulator on stack
uint8_t PHP(); //Push processor status on stack
uint8_t PLA(); //Pull accumulator from stack
uint8_t PLP(); //Pull processor status from stack
uint8_t AND(); //Logical AND
uint8_t EOR(); //Exclusive OR
uint8_t ORA(); //Logical Inclusive OR
uint8_t BIT(); //Bit Test
uint8_t ADC(); //Add with Carry
uint8_t SBC(); //Substract with Carry
uint8_t CMP(); //Compare accumulator
uint8_t CPX(); //Compare X register
uint8_t CPY(); //Compare Y register
uint8_t INC(); //Increment a memory location
uint8_t INX(); //Increment the X register
uint8_t INY(); //Increment the Y register
uint8_t DEC(); //Decrement a memory location
uint8_t DEX(); //Decrement the X register
uint8_t DEY(); //Decrement the Y register
uint8_t ASL(); //Arithmetic Shift Left
uint8_t LSR(); //Logical Shift Right
uint8_t ROL(); //Rotate Left
uint8_t ROR(); //Rotate Right
uint8_t JMP(); //Jump to another location
uint8_t JSR(); //Jump to a subroutine
uint8_t RTS(); //Return from subroutine
uint8_t BCC(); //Branch if carry flag clear
uint8_t BCS(); //Branch if carry flag set
uint8_t BEQ(); //Branch if zero flag set
uint8_t BMI(); //Branch if negative flag set
uint8_t BNE(); //Branch if zero flag clear
uint8_t BPL(); //Branch if negative flag clear
uint8_t BVC(); //Branch if overflow flag clear
uint8_t BVS(); //Branch if overflow flag set
uint8_t CLC(); //Clear carry flag
uint8_t CLD(); //Clear decimal mode flag
uint8_t CLI(); //Clear interrupt flag
uint8_t CLV(); //Clear overflow flag
uint8_t SEC(); //Set carry flag
uint8_t SED(); //Set decimal mode flag
uint8_t SEI(); //Set interrupt disable flag
uint8_t BRK(); //Force an interrupt
uint8_t NOP(); //No Operations
uint8_t RTI(); //Return from Interrupt

void reset(uint16_t address);
int clock();
void run();

#endif //VIRTUAL_MACHINE_CPU6052_H
