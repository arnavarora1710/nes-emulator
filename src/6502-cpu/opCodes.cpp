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

uint8_t ISA::ASL() {
    m_cpu.fetch();
    const auto result = static_cast<uint16_t>(m_cpuState.fetched) << 1;
    m_cpu.setFlag(CPU::StatusBit::CARRY_BIT, (result & 0xFF00) > 0);
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, (result & 0x00FF) == 0x00);
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, result & 0x80);

    const auto instr = m_cpu.m_isa.getInstruction(m_cpuState.opcode);
    if (instr.addrMode != &ISA::IMP) {
        m_registers.A = result & 0x00FF;
    } else {
        m_cpu.write(m_cpuState.absAddr, result & 0x00FF);
    }

    return 0;
}

uint8_t ISA::BCC() {
    if (m_cpu.getFlag(CPU::StatusBit::CARRY_BIT) == 0) {
        m_cpuState.cycles++;
        m_cpuState.absAddr = m_registers.PC + m_cpuState.relAddr;

        const auto absAddrHi = m_cpuState.absAddr & 0xFF00;
        const auto pcHi = m_registers.PC & 0xFF00;
        if (absAddrHi != pcHi) m_cpuState.cycles++;

        m_registers.PC = m_cpuState.absAddr;
    }
    return 0;
}

uint8_t ISA::BCS() {
    if (m_cpu.getFlag(CPU::StatusBit::CARRY_BIT) == 1) {
        m_cpuState.cycles++;
        m_cpuState.absAddr = m_registers.PC + m_cpuState.relAddr;

        const auto absAddrHi = m_cpuState.absAddr & 0xFF00;
        const auto pcHi = m_registers.PC & 0xFF00;
        if (absAddrHi != pcHi) m_cpuState.cycles++;

        m_registers.PC = m_cpuState.absAddr;
    }
    return 0;
}

uint8_t ISA::BEQ() {
    if (m_cpu.getFlag(CPU::StatusBit::ZERO_BIT) == 1) {
        m_cpuState.cycles++;
        m_cpuState.absAddr = m_registers.PC + m_cpuState.relAddr;

        const auto absAddrHi = m_cpuState.absAddr & 0xFF00;
        const auto pcHi = m_registers.PC & 0xFF00;
        if (absAddrHi != pcHi) m_cpuState.cycles++;

        m_registers.PC = m_cpuState.absAddr;
    }
    return 0;
}

uint8_t ISA::BIT() {
    m_cpu.fetch();
    const auto result = m_cpuState.fetched & m_registers.A;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, (result & 0x00FF) == 0x00);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, (m_cpuState.fetched << 7));
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, (m_cpuState.fetched << 6));
    return 0;
}

uint8_t ISA::BMI() {
    if (m_cpu.getFlag(CPU::StatusBit::SIGN_BIT) == 1) {
        m_cpuState.cycles++;
        m_cpuState.absAddr = m_registers.PC + m_cpuState.relAddr;

        const auto absAddrHi = m_cpuState.absAddr & 0xFF00;
        const auto pcHi = m_registers.PC & 0xFF00;
        if (absAddrHi != pcHi) m_cpuState.cycles++;

        m_registers.PC = m_cpuState.absAddr;
    }
    return 0;
}

uint8_t ISA::BNE() {
    if (m_cpu.getFlag(CPU::StatusBit::ZERO_BIT) == 0) {
        m_cpuState.cycles++;
        m_cpuState.absAddr = m_registers.PC + m_cpuState.relAddr;

        const auto absAddrHi = m_cpuState.absAddr & 0xFF00;
        const auto pcHi = m_registers.PC & 0xFF00;
        if (absAddrHi != pcHi) m_cpuState.cycles++;

        m_registers.PC = m_cpuState.absAddr;
    }
    return 0;
}

uint8_t ISA::BPL() {
    if (m_cpu.getFlag(CPU::StatusBit::SIGN_BIT) == 0) {
        m_cpuState.cycles++;
        m_cpuState.absAddr = m_registers.PC + m_cpuState.relAddr;

        const auto absAddrHi = m_cpuState.absAddr & 0xFF00;
        const auto pcHi = m_registers.PC & 0xFF00;
        if (absAddrHi != pcHi) m_cpuState.cycles++;

        m_registers.PC = m_cpuState.absAddr;
    }
    return 0;
}

// TODO
uint8_t ISA::BRK() { return 0; }

uint8_t ISA::BVC() {
    if (m_cpu.getFlag(CPU::StatusBit::OVERFLOW_BIT) == 0) {
        m_cpuState.cycles++;
        m_cpuState.absAddr = m_registers.PC + m_cpuState.relAddr;

        const auto absAddrHi = m_cpuState.absAddr & 0xFF00;
        const auto pcHi = m_registers.PC & 0xFF00;
        if (absAddrHi != pcHi) m_cpuState.cycles++;

        m_registers.PC = m_cpuState.absAddr;
    }
    return 0;
}

uint8_t ISA::BVS() {
    if (m_cpu.getFlag(CPU::StatusBit::OVERFLOW_BIT) == 1) {
        m_cpuState.cycles++;
        m_cpuState.absAddr = m_registers.PC + m_cpuState.relAddr;

        const auto absAddrHi = m_cpuState.absAddr & 0xFF00;
        const auto pcHi = m_registers.PC & 0xFF00;
        if (absAddrHi != pcHi) m_cpuState.cycles++;

        m_registers.PC = m_cpuState.absAddr;
    }
    return 0;
}

uint8_t ISA::CLC() {
    m_cpu.setFlag(CPU::StatusBit::CARRY_BIT, false);
    return 0;
}

uint8_t ISA::CLD() {
    m_cpu.setFlag(CPU::StatusBit::DECIMAL_BIT, false);
    return 0;
}

uint8_t ISA::CLI() {
    m_cpu.setFlag(CPU::StatusBit::INTERRUPT_DISABLE_BIT, false);
    return 0;
}

uint8_t ISA::CLV() {
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, false);
    return 0;
}

uint8_t ISA::CMP() {
    m_cpu.fetch();
    const auto result = static_cast<uint16_t>(m_registers.A - m_cpuState.fetched);
    m_cpu.setFlag(CPU::StatusBit::CARRY_BIT, m_registers.A >= m_cpuState.fetched);
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, (result & 0x00FF) == 0);
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, result & 0x0080);
    return 1;
}

uint8_t ISA::CPX() {
    m_cpu.fetch();
    const auto result = static_cast<uint16_t>(m_registers.X - m_cpuState.fetched);
    m_cpu.setFlag(CPU::StatusBit::CARRY_BIT, m_registers.X >= m_cpuState.fetched);
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, (result & 0x00FF) == 0);
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, result & 0x0080);
    return 1;
}

uint8_t ISA::CPY() {
    m_cpu.fetch();
    const auto result = static_cast<uint16_t>(m_registers.Y - m_cpuState.fetched);
    m_cpu.setFlag(CPU::StatusBit::CARRY_BIT, m_registers.Y >= m_cpuState.fetched);
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, (result & 0x00FF) == 0);
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, result & 0x0080);
    return 1;
}

uint8_t ISA::DEC() {
    m_cpu.fetch();
    const auto result = m_cpuState.fetched - 1;
    m_cpu.write(m_cpuState.absAddr, result & 0x00FF);
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, (result & 0x00FF) == 0x0000);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, result & 0x0080);
    return 0;
}

uint8_t ISA::DEX() {
    m_registers.X--;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.X == 0);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.X & 0x80);
    return 0;
}

uint8_t ISA::DEY() {
    m_registers.Y--;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.Y == 0);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.Y & 0x80);
    return 0;
}

uint8_t ISA::EOR() {
    m_cpu.fetch();
    m_registers.A = m_registers.A ^ m_cpuState.fetched;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.A == 0);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.A & 0x80);
    return 1;
}

uint8_t ISA::INC() {
    m_cpu.fetch();
    const auto result = m_cpuState.fetched + 1;
    m_cpu.write(m_cpuState.absAddr, result & 0x00FF);
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, (result & 0x00FF) == 0x0000);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, result & 0x0080);
    return 0;
}

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

uint8_t ISA::LSR() {
    m_cpu.fetch();
    m_cpu.setFlag(CPU::StatusBit::CARRY_BIT, m_cpuState.fetched & 0x0001);
    const auto result = static_cast<uint16_t>(m_cpuState.fetched) >> 1;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, (result & 0x00FF) == 0x00);
    m_cpu.setFlag(CPU::StatusBit::OVERFLOW_BIT, result & 0x80);

    const auto instr = m_cpu.m_isa.getInstruction(m_cpuState.opcode);
    if (instr.addrMode != &ISA::IMP) {
        m_registers.A = result & 0x00FF;
    } else {
        m_cpu.write(m_cpuState.absAddr, result & 0x00FF);
    }
    return 0;
}

uint8_t ISA::NOP() {
    switch (m_cpuState.opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            return 1;
        default: ;
    }
    return 0;
}

uint8_t ISA::ORA() {
    m_cpu.fetch();
    m_registers.A = m_registers.A | m_cpuState.fetched;
    m_cpu.setFlag(CPU::StatusBit::ZERO_BIT, m_registers.A == 0);
    m_cpu.setFlag(CPU::StatusBit::SIGN_BIT, m_registers.A & 0x80);
    return 1;
}

uint8_t ISA::PHA() { return 0; }
uint8_t ISA::PHP() { return 0; } uint8_t ISA::PLA() { return 0; } uint8_t ISA::PLP() { return 0; }
uint8_t ISA::ROL() { return 0; }
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
