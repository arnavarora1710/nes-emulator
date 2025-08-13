#include "isa.hpp"
#include "cpu.hpp"

uint8_t ISA::ADC() {
    m_cpu.fetch();
    const auto result = static_cast<uint16_t>(
        m_registers.A +
        m_cpuState.fetched +
        m_cpu.getFlag(CPU::StatusBit::CARRY_BIT)
    );
    m_cpu.setFlag(CPU::StatusBit::CARRY_BIT, result > 255);
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, (result & 0x00FF) == 0);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, result & 0x80);

    // overflow happens if
    // both positive and result negative
    // both negative and result positive
    // "both" meaning the accumulator and the value fetched here

    // so writing out the truth table,
    // overflow happens if
    const auto tmp1 = static_cast<uint16_t>(m_registers.A ^ m_cpuState.fetched);
    const auto tmp2 = static_cast<uint16_t>(m_registers.A ^ result);
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, ~tmp1 & tmp2 & 0x0080);

    m_registers.A = result & 0x00FF;
    return 1;
}

uint8_t ISA::AND() {
    m_cpu.fetch();
    m_registers.A &= m_cpuState.fetched;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.A == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.A & 0x80);
    return 1;
}

uint8_t ISA::ASL() { return 0; } uint8_t ISA::BCC() { return 0; }
uint8_t ISA::BCS() { return 0; } uint8_t ISA::BEQ() { return 0; } uint8_t ISA::BIT() { return 0; } uint8_t ISA::BMI() { return 0; }
uint8_t ISA::BNE() { return 0; } uint8_t ISA::BPL() { return 0; } uint8_t ISA::BRK() { return 0; } uint8_t ISA::BVC() { return 0; }
uint8_t ISA::BVS() { return 0; } uint8_t ISA::CLC() { return 0; } uint8_t ISA::CLD() { return 0; } uint8_t ISA::CLI() { return 0; }
uint8_t ISA::CLV() { return 0; } uint8_t ISA::CMP() { return 0; } uint8_t ISA::CPX() { return 0; } uint8_t ISA::CPY() { return 0; }
uint8_t ISA::DEC() { return 0; } uint8_t ISA::DEX() { return 0; } uint8_t ISA::DEY() { return 0; } uint8_t ISA::EOR() { return 0; }
uint8_t ISA::INC() { return 0; }

uint8_t ISA::INX() {
    m_registers.X++;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.X == 0);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.X & 0x80);
    return 0;
}

uint8_t ISA::INY() {
    m_registers.Y++;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.Y == 0);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.Y & 0x80);
    return 0;
}

uint8_t ISA::JMP() {
    m_registers.PC = m_cpuState.absAddr;
    return 0;
}

// TODO
uint8_t ISA::JSR() {
    return 0;
}

uint8_t ISA::LDA() {
    m_cpu.fetch();
    m_registers.A = m_cpuState.fetched;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.A == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.A & 0x80);
    return 1;
}

uint8_t ISA::LDX() {
    m_cpu.fetch();
    m_registers.X = m_cpuState.fetched;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.X == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.X & 0x80);
    return 1;
}

uint8_t ISA::LDY() {
    m_cpu.fetch();
    m_registers.Y = m_cpuState.fetched;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.Y == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.Y & 0x80);
    return 1;
}
uint8_t ISA::LSR() { return 0; } uint8_t ISA::NOP() { return 0; } uint8_t ISA::ORA() { return 0; } uint8_t ISA::PHA() { return 0; }
uint8_t ISA::PHP() { return 0; } uint8_t ISA::PLA() { return 0; } uint8_t ISA::PLP() { return 0; } uint8_t ISA::ROL() { return 0; }
uint8_t ISA::ROR() { return 0; } uint8_t ISA::RTI() { return 0; } uint8_t ISA::RTS() { return 0; }

uint8_t ISA::SBC() {
    m_cpu.fetch();
    const auto tmp = static_cast<uint16_t>(m_cpuState.fetched) ^ 0x00FF;
    const auto result = static_cast<uint16_t>(
        m_registers.A +
        tmp +
        m_cpu.getFlag(CPU::StatusBit::CARRY_BIT)
    );
    m_cpu.setFlag(CPU::StatusBit::CARRY_BIT, result & 0xFF00);
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, (result & 0x00FF) == 0);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, result & 0x0080);

    // overflow happens if
    // both positive and result negative
    // both negative and result positive
    // "both" meaning the accumulator and the value fetched here

    // so writing out the truth table,
    // overflow happens if
    const auto tmp1 = static_cast<uint16_t>(result ^ tmp);
    const auto tmp2 = static_cast<uint16_t>(result ^ m_registers.A);
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, tmp1 & tmp2 & 0x0080);

    m_registers.A = result & 0x00FF;
    return 1;
}

uint8_t ISA::SEC() {
    m_cpu.setFlag(CPU::StatusBit::CARRY_BIT, true);
    return 0;
}

uint8_t ISA::SED() {
    m_cpu.setFlag(CPU::StatusBit::DECIMAL_BIT, true);
    return 0;
}

uint8_t ISA::SEI() {
    m_cpu.setFlag(CPU::StatusBit::INTERRUPT_DISABLE_BIT, true);
    return 0;
}

uint8_t ISA::STA() {
    m_cpu.write(m_cpuState.absAddr, m_registers.A);
    return 0;
}

uint8_t ISA::STX() {
    m_cpu.write(m_cpuState.absAddr, m_registers.X);
    return 0;
}

uint8_t ISA::STY() {
    m_cpu.write(m_cpuState.absAddr, m_registers.Y);
    return 0;
}

uint8_t ISA::TAX() {
    m_registers.X = m_registers.A;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.X == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.X & 0x80);
    return 0;
}

uint8_t ISA::TAY() {
    m_registers.Y = m_registers.A;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.Y == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.Y & 0x80);
    return 0;
}

uint8_t ISA::TSX() {
    m_registers.X = m_registers.SP;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.X == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.X & 0x80);
    return 0;
}

uint8_t ISA::TXA() {
    m_registers.A = m_registers.X;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.A == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.A & 0x80);
    return 0;
}

uint8_t ISA::TXS() {
    m_registers.SP = m_registers.X;
    return 0;
}

uint8_t ISA::TYA() {
    m_registers.A = m_registers.Y;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.A == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.A & 0x80);
    return 0;
}

uint8_t ISA::XXX() {
    return 0;
}
