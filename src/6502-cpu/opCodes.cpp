#include "isa.hpp"
#include "cpu.hpp"

uint8_t ISA::AND() {
    m_cpu.fetch();
    m_registers.A &= m_cpuState.fetched;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.A == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.A & 0x80);
    return 1;
}

uint8_t ISA::ADC() { return 0; } uint8_t ISA::ASL() { return 0; } uint8_t ISA::BCC() { return 0; }
uint8_t ISA::BCS() { return 0; } uint8_t ISA::BEQ() { return 0; } uint8_t ISA::BIT() { return 0; } uint8_t ISA::BMI() { return 0; }
uint8_t ISA::BNE() { return 0; } uint8_t ISA::BPL() { return 0; } uint8_t ISA::BRK() { return 0; } uint8_t ISA::BVC() { return 0; }
uint8_t ISA::BVS() { return 0; } uint8_t ISA::CLC() { return 0; } uint8_t ISA::CLD() { return 0; } uint8_t ISA::CLI() { return 0; }
uint8_t ISA::CLV() { return 0; } uint8_t ISA::CMP() { return 0; } uint8_t ISA::CPX() { return 0; } uint8_t ISA::CPY() { return 0; }
uint8_t ISA::DEC() { return 0; } uint8_t ISA::DEX() { return 0; } uint8_t ISA::DEY() { return 0; } uint8_t ISA::EOR() { return 0; }
uint8_t ISA::INC() { return 0; } uint8_t ISA::INX() { return 0; } uint8_t ISA::INY() { return 0; } uint8_t ISA::JMP() { return 0; }
uint8_t ISA::JSR() { return 0; } uint8_t ISA::LDA() { return 0; } uint8_t ISA::LDX() { return 0; } uint8_t ISA::LDY() { return 0; }
uint8_t ISA::LSR() { return 0; } uint8_t ISA::NOP() { return 0; } uint8_t ISA::ORA() { return 0; } uint8_t ISA::PHA() { return 0; }
uint8_t ISA::PHP() { return 0; } uint8_t ISA::PLA() { return 0; } uint8_t ISA::PLP() { return 0; } uint8_t ISA::ROL() { return 0; }
uint8_t ISA::ROR() { return 0; } uint8_t ISA::RTI() { return 0; } uint8_t ISA::RTS() { return 0; } uint8_t ISA::SBC() { return 0; }
uint8_t ISA::SEC() { return 0; } uint8_t ISA::SED() { return 0; } uint8_t ISA::SEI() { return 0; } uint8_t ISA::STA() { return 0; }
uint8_t ISA::STX() { return 0; } uint8_t ISA::STY() { return 0; } uint8_t ISA::TAX() { return 0; } uint8_t ISA::TAY() { return 0; }
uint8_t ISA::TSX() { return 0; } uint8_t ISA::TXA() { return 0; } uint8_t ISA::TXS() { return 0; } uint8_t ISA::TYA() { return 0; }
uint8_t ISA::XXX() { return 0; }
