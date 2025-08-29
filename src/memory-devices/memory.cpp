#include <string>

#include "memory.hpp"

Memory::Memory(const std::size_t size,
               const uint16_t start_addr,
               const uint16_t mirror_mask)
    : memory_size(size)
    , memory_start_addr(start_addr)
    , memory_mirror_mask(mirror_mask)
{
    memory_end_addr = memory_start_addr + memory_size - 1;
    memory.resize(memory_size);
}

uint8_t Memory::read(const uint16_t address) {
    if (!isAddressInRange(address)) {
        throw std::out_of_range("Address out of range in Memory: " + std::to_string(address));
    }
    return memory[(address - memory_start_addr) & memory_mirror_mask];
}

void Memory::write(const uint16_t address, const uint8_t data) {
    if (!isAddressInRange(address)) {
        throw std::out_of_range("Address out of range in Memory: " + std::to_string(address));
    }
    memory[(address - memory_start_addr) & memory_mirror_mask] = data;
}

void Memory::loadROM(const json& rom_test) {
    const auto& initial_state = rom_test["initial"];
    const auto& initial_ram = initial_state["ram"];

    for (const auto& memory_desc : initial_ram) {
        const std::size_t& address = memory_desc[0];
        const int& data = memory_desc[1];
        memory[address] = data;
    }
}

bool Memory::checkFinalState(const json &rom_test) const {
    const auto& final_state = rom_test["final"];
    const auto& final_ram = final_state["ram"];

    for (const auto& memory_desc : final_ram) {
        const std::size_t& address = memory_desc[0];
        const uint8_t expected = memory_desc[1];
        const uint8_t actual = memory[address];
        if (actual != expected) {
            return false;
        }
    }
    return true;
}
