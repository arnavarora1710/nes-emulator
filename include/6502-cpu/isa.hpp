#pragma once

#include <functional>
#include <string>
#include <vector>

using Register = uint8_t;
using ProgramCounter = uint16_t;

struct Registers {
    // Registers for CPU state and execution storage
    Register A, X, Y, SP;
    ProgramCounter PC;

    // status byte with 8 bits, each representing a different status flag
    Register Status; 
};

struct CPUState {
    uint8_t opcode { 0 };
    uint8_t fetched { 0 };
    uint16_t temp { 0 };
    uint16_t absAddr { 0 };
    uint16_t relAddr { 0 };
    uint8_t cycles { 0 };
};

class CPU;

struct ISA {
    using InstructionFunc = uint8_t(ISA::*)();
    // Addressing modes
    uint8_t IMP(); uint8_t IMM();
    uint8_t ZP0(); uint8_t ZPX();
    uint8_t ZPY(); uint8_t REL();
    uint8_t ABS(); uint8_t ABX();
    uint8_t ABY(); uint8_t IND();
    uint8_t IZX(); uint8_t IZY();

    // Opcodes
    uint8_t ADC(); uint8_t AND(); uint8_t ASL(); uint8_t BCC();
    uint8_t BCS(); uint8_t BEQ(); uint8_t BIT(); uint8_t BMI();
    uint8_t BNE(); uint8_t BPL(); uint8_t BRK(); uint8_t BVC();
    uint8_t BVS(); uint8_t CLC(); uint8_t CLD(); uint8_t CLI();
    uint8_t CLV(); uint8_t CMP(); uint8_t CPX(); uint8_t CPY();
    uint8_t DEC(); uint8_t DEX(); uint8_t DEY(); uint8_t EOR();
    uint8_t INC(); uint8_t INX(); uint8_t INY(); uint8_t JMP();
    uint8_t JSR(); uint8_t LDA(); uint8_t LDX(); uint8_t LDY();
    uint8_t LSR(); uint8_t NOP(); uint8_t ORA(); uint8_t PHA();
    uint8_t PHP(); uint8_t PLA(); uint8_t PLP(); uint8_t ROL();
    uint8_t ROR(); uint8_t RTI(); uint8_t RTS(); uint8_t SBC();
    uint8_t SEC(); uint8_t SED(); uint8_t SEI(); uint8_t STA();
    uint8_t STX(); uint8_t STY(); uint8_t TAX(); uint8_t TAY();
    uint8_t TSX(); uint8_t TXA(); uint8_t TXS(); uint8_t TYA();

    uint8_t XXX();

    struct Instruction {
        std::string name;
        InstructionFunc operation;
        InstructionFunc addrMode;
        std::uint8_t cycles;
    };

    ISA() = delete;
    explicit ISA(CPU& cpu);

    ~ISA() = default;

    [[nodiscard]] Instruction getInstruction(uint8_t opcode) const;
    [[nodiscard]] uint8_t execute(uint8_t opcode);

    std::vector<Instruction> m_instructions;
    CPU& m_cpu;
    Registers& m_registers;
    CPUState& m_cpuState;
};
