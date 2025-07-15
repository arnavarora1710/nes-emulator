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
    return (Status & static_cast<uint8_t>(bit)) ? 1 : 0;
}

void CPU::setFlag(StatusBit bit, bool value)
{
    if (value)
        Status |= static_cast<uint8_t>(bit);
    else
        Status &= ~static_cast<uint8_t>(bit);
}
