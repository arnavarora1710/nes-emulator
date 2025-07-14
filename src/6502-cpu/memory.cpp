#include <string>

#include "memory.hpp"

uint8_t Memory::read(uint16_t address) {
    if (!isAddressInRange(address)) {
        throw std::out_of_range("Address out of range in Memory: " + std::to_string(address));
    }
    return memory[address - MEMORY_START_ADDRESS];
}

void Memory::write(uint16_t address, uint8_t data) {
    if (!isAddressInRange(address)) {
        throw std::out_of_range("Address out of range in Memory: " + std::to_string(address));
    }
    memory[address - MEMORY_START_ADDRESS] = data;
}
