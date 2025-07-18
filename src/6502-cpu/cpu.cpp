#include <cstdint>

#include "bus.hpp"
#include "cpu.hpp"

uint8_t CPU::read(uint16_t address)
{
    return m_bus.read(address);
}

void CPU::write(uint16_t address, uint8_t data)
{
    m_bus.write(address, data);
}

uint8_t CPU::getFlag(StatusBit bit) const
{
    return (m_registers.Status & static_cast<uint8_t>(bit)) ? 1 : 0;
}

void CPU::setFlag(StatusBit bit, bool value)
{
    if (value)
        m_registers.Status |= static_cast<uint8_t>(bit);
    else
        m_registers.Status &= ~static_cast<uint8_t>(bit);
}

uint8_t CPU::fetch() {
    const auto& instr = m_isa.getInstruction(m_cpuState.opcode);
    if (instr.addrMode != &ISA::IMP) {
        m_cpuState.fetched = read(m_cpuState.absAddr);
    }
    return m_cpuState.fetched;
}

void CPU::clock() {
    if (m_cpuState.cycles == 0) {
        const uint8_t opcode = read(m_registers.PC++);
        setFlag(StatusBit::UNUSED_BIT, true);
        m_cpuState.cycles = m_isa.execute(opcode);
        setFlag(StatusBit::UNUSED_BIT, true);
    }
    m_cpuState.cycles--;
}
