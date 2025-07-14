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
