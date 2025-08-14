#include <iostream>
#include <iomanip>

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

void CPU::reset() {
    m_cpuState.absAddr = 0xFFFC;
    const auto lo = read(m_cpuState.absAddr);
    const auto hi = read(m_cpuState.absAddr + 1);

    m_registers.PC = (lo << 8) | hi;

    m_registers.A = m_registers.X = m_registers.Y = m_registers.Status = 0;
    setFlag(StatusBit::UNUSED_BIT, true);

    m_cpuState.absAddr = m_cpuState.relAddr = 0x0000;
    m_cpuState.fetched = 0x00;

    // Resetting state takes time
    m_cpuState.cycles = 0;
}

void CPU::interrupt() {
    if (getFlag(StatusBit::INTERRUPT_DISABLE_BIT) == 0) {

        write(STK_PTR_OFFSET + m_registers.SP, (m_registers.PC >> 8) & 0x00FF);
        m_registers.SP--;
        write(STK_PTR_OFFSET + m_registers.SP, m_registers.PC & 0x00FF);
        m_registers.SP--;

        setFlag(StatusBit::BREAK_CMD_BIT, false);
        setFlag(StatusBit::UNUSED_BIT, true);
        setFlag(StatusBit::INTERRUPT_DISABLE_BIT, true);

        write(STK_PTR_OFFSET + m_registers.SP, m_registers.Status);
        m_registers.SP--;

        m_cpuState.absAddr = 0xFFFE;
        const auto addrLo = read(m_cpuState.absAddr);
        const auto addrHi = read(m_cpuState.absAddr + 1);
        m_registers.PC = (addrHi << 8) | addrLo;

        m_cpuState.cycles = 7;
    }
}

void CPU::nmi() {
    write(STK_PTR_OFFSET + m_registers.SP, (m_registers.PC >> 8) & 0x00FF);
    m_registers.SP--;
    write(STK_PTR_OFFSET + m_registers.SP, m_registers.PC & 0x00FF);
    m_registers.SP--;

    setFlag(StatusBit::BREAK_CMD_BIT, false);
    setFlag(StatusBit::UNUSED_BIT, true);
    setFlag(StatusBit::INTERRUPT_DISABLE_BIT, true);

    write(STK_PTR_OFFSET + m_registers.SP, m_registers.Status);
    m_registers.SP--;

    m_cpuState.absAddr = 0xFFFA;
    const auto addrLo = read(m_cpuState.absAddr);
    const auto addrHi = read(m_cpuState.absAddr + 1);
    m_registers.PC = (addrHi << 8) | addrLo;

    m_cpuState.cycles = 8;
}

template <typename T>
void print_hex_value(const std::string_view name, T value) {
    std::cout << name << ": 0x"
                  << std::hex << std::uppercase
                  << std::setw(sizeof(T) * 2)
                  << std::setfill('0')
                  << static_cast<uint64_t>(value)
                  << std::dec << '\n';
}

void CPU::print_cpu_state() const {
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Registers" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    print_hex_value("Program Counter (PC)", m_registers.PC);
    print_hex_value("Accumulator (A)", m_registers.A);
    print_hex_value("Register X", m_registers.X);
    print_hex_value("Register Y", m_registers.Y);
    for (const auto& [bit, bit_name] : status_bit_names) {
        if (getFlag(bit))
            std::cout << bit_name << " is set" << std::endl;
        else
            std::cout << bit_name << " is clear" << std::endl;
    }
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "CPU State" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    print_hex_value("Number of Cycles", m_cpuState.cycles);
    print_hex_value("Current Value Fetched", m_cpuState.fetched);
    print_hex_value("Absolute Address", m_cpuState.absAddr);
    print_hex_value("Relative Address", m_cpuState.relAddr);
    print_hex_value("Current Opcode", m_cpuState.opcode);
    std::cout << "Current Instruction: " << m_isa.getInstruction(m_cpuState.opcode).name << std::endl;
    std::cout << std::string(50, '-') << std::endl;
}