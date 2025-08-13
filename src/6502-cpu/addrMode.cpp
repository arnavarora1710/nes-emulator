#include "cpu.hpp"
#include "isa.hpp"

// Implied Addressing
// What needs to be accessed is implied by the instruction
uint8_t ISA::IMP() {
    // special case for PHA
    // needs the accumulator
    m_cpuState.fetched = m_registers.A;
    return 0; 
}

// Immediate Addressing
// Literal operand given right after the address mode
uint8_t ISA::IMM() {
    // Move to the next byte to grab the literal
    m_cpuState.absAddr = m_registers.PC++;
    return 0;
}

// Absolute Addressing
// Access memory at the given address (need to store in uint16_t)
uint8_t ISA::ABS() {
    const uint16_t loByte = m_cpu.read(m_registers.PC++);
    const uint16_t hiByte = m_cpu.read(m_registers.PC++);
    m_cpuState.absAddr = (hiByte << 8) | loByte;
    return 0;
}

// Indexed Addressing: Absolute X
// Similar to ABS but add contents of X
// Also, if the address spills to the next page, return 1 for
// an additional clock cycle
uint8_t ISA::ABX() {
    const uint16_t highByte = m_cpu.read(m_registers.PC++);
    const uint16_t lowByte = m_cpu.read(m_registers.PC++);

    m_cpuState.absAddr = (highByte << 8) | lowByte;
    m_cpuState.absAddr += m_registers.X;

    if ((m_cpuState.absAddr & 0xFF00) != (highByte << 8)) {
        return 1;
    }

    return 0;
}

// Indexed Addressing: Absolute Y
// Similar to ABS but add contents of Y
// Also, if the address spills to the next page, return 1 for
// an additional clock cycle
uint8_t ISA::ABY() {
    const uint16_t highByte = m_cpu.read(m_registers.PC++);
    const uint16_t lowByte = m_cpu.read(m_registers.PC++);

    m_cpuState.absAddr = (highByte << 8) | lowByte;
    m_cpuState.absAddr += m_registers.Y;

    if ((m_cpuState.absAddr & 0xFF00) != (highByte << 8))
        return 1;

    return 0;
}

// Zero Page Addressing
// Read from the Zero Page (first 256 bytes)
// Takes 1 less cycle as the address to be read is only 1 byte now
uint8_t ISA::ZP0() {
    m_cpuState.absAddr = m_cpu.read(m_registers.PC++);
    m_cpuState.absAddr &= 0x00FF;
    return 0;
}

// Indexed Addressing: Zero Page X
// Read from the Zero Page (first 256 bytes) and add contents of X
// Takes 1 less cycle as the address to be read is only 1 byte now
uint8_t ISA::ZPX() {
    m_cpuState.absAddr = m_cpu.read(m_registers.PC++) + m_registers.X;
    m_cpuState.absAddr &= 0x00FF;
    return 0;
}

// Indexed Addressing: Zero Page Y
// Read from the Zero Page (first 256 bytes) and add contents of Y
// Takes 1 less cycle as the address to be read is only 1 byte now
uint8_t ISA::ZPY() {
    m_cpuState.absAddr = m_cpu.read(m_registers.PC++) + m_registers.Y;
    m_cpuState.absAddr &= 0x00FF;
    return 0;
}

// Relative Addressing
// Only for conditional branch jumps
// Can only jump half a page backwards or forwards (-128 to +127)
uint8_t ISA::REL() {
    m_cpuState.relAddr = m_cpu.read(m_registers.PC++);
    if (m_cpuState.relAddr & 0x80)
        m_cpuState.relAddr |= 0xFF00;
    return 0;
}

// Indirect Addressing
// Read the address which stores the address of the data
// Has a bug in its implementation on hardware
// So have to emulate that too
uint8_t ISA::IND() {
    const uint16_t readAddressLo = m_cpu.read(m_registers.PC++);
    const uint16_t readAddressHi = m_cpu.read(m_registers.PC++);
    const uint16_t readAddress = (readAddressHi << 8) | readAddressLo;

    if (readAddressLo == 0x00FF) {
        // Simulate hardware bug
        const uint16_t actualAddressLo = m_cpu.read(readAddress);
        const uint16_t actualAddressHi = m_cpu.read(readAddress & 0xFF00);
        m_cpuState.absAddr = (actualAddressHi << 8) | actualAddressLo;
    } else {
        const uint16_t actualAddressLo = m_cpu.read(readAddress);
        const uint16_t actualAddressHi = m_cpu.read(readAddress + 1);
        m_cpuState.absAddr = (actualAddressHi << 8) | actualAddressLo;
    }

    return 0;
}

// Pre-Indexed Indirect X
// Works a lot like Zero Page X
// but needs to do an additional lookup
// Also indexes a location in the zero page
uint8_t ISA::IZX() {
    const uint16_t readAddress = m_cpu.read(m_registers.PC++);

    const uint16_t xReg = m_registers.X;
    const uint16_t addrLo = (readAddress + xReg) & 0x00FF;
    const uint16_t addrHi = (readAddress + xReg + static_cast<uint16_t>(1)) & 0x00FF;
    const uint16_t readAddressLo = m_cpu.read(addrLo);
    const uint16_t readAddressHi = m_cpu.read(addrHi);

    m_cpuState.absAddr = (readAddressHi << 8) | readAddressLo;

    return 0;
}

// Post-Indexed Indirect Y
// Works similarly, but the offset is done
// after the lookup
// If a page boundary is crossed, an additional
// cycle is required
uint8_t ISA::IZY() {
    const uint16_t readAddress = m_cpu.read(m_registers.PC++);
    const uint16_t addrLo = readAddress & 0x00FF;
    const uint16_t addrHi = (readAddress + static_cast<uint16_t>(1)) & 0x00FF;
    const uint16_t readAddressLo = m_cpu.read(addrLo);
    const uint16_t readAddressHi = m_cpu.read(addrHi);

    m_cpuState.absAddr = (readAddressHi << 8) | readAddressLo;
    m_cpuState.absAddr += m_registers.Y;

    if ((m_cpuState.absAddr & 0xFF00) != (readAddressHi << 8))
        return 1;

    return 0;
}
