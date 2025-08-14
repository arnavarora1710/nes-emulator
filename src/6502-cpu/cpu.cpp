#include <fstream>
#include <iostream>

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
void print_cpu_value(const std::string_view name, T value) {
    std::cout << name << " = " << static_cast<uint64_t>(value) << std::endl;
}

void CPU::print_cpu_state() const {
    std::cout << std::endl << std::string(50, '-') << std::endl;
    std::cout << "Registers" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    print_cpu_value("Program Counter (PC)", m_registers.PC);
    print_cpu_value("Accumulator (A)", m_registers.A);
    print_cpu_value("Register X", m_registers.X);
    print_cpu_value("Register Y", m_registers.Y);
    print_cpu_value("Status", m_registers.Status);
    for (const auto& [bit, bit_name] : status_bit_names) {
        if (getFlag(bit))
            std::cout << bit_name << " is set" << std::endl;
        else
            std::cout << bit_name << " is clear" << std::endl;
    }
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "CPU State" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    print_cpu_value("Number of Cycles", m_cpuState.cycles);
    print_cpu_value("Current Value Fetched", m_cpuState.fetched);
    print_cpu_value("Absolute Address", m_cpuState.absAddr);
    print_cpu_value("Relative Address", m_cpuState.relAddr);
    print_cpu_value("Current Opcode", m_cpuState.opcode);
    std::cout << "Current Instruction: " << m_isa.getInstruction(m_cpuState.opcode).name << std::endl;
    std::cout << std::string(50, '-') << std::endl << std::endl;
}

void CPU::init_with_rom(const json& rom_test) {
    const auto& initial_state = rom_test["initial"];

    m_registers.PC = initial_state["pc"];
    m_registers.A = initial_state["a"];
    m_registers.X = initial_state["x"];
    m_registers.Y = initial_state["y"];
    m_registers.SP = initial_state["s"];
    m_registers.Status = initial_state["p"];
}

bool CPU::check_final_state(const json& rom_test) {
    const auto& final_state = rom_test["final"];
    return m_registers.PC == final_state["pc"] and
           m_registers.A == final_state["a"] and
           m_registers.X == final_state["x"] and
           m_registers.Y == final_state["y"] and
           m_registers.SP == final_state["s"] and
           m_registers.Status == final_state["p"];
}