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

void Memory::load_rom(const json& rom_test) {
    const auto& initial_state = rom_test["initial"];
    const auto& initial_ram = initial_state["ram"];

    for (const auto& memory_desc : initial_ram) {
        const std::size_t& address = memory_desc[0];
        const int& data = memory_desc[1];
        memory[address] = data;
    }
}

bool Memory::check_final_state(const json &rom_test) {
    const auto& final_state = rom_test["final"];
    const auto& final_ram = final_state["ram"];

    for (const auto& memory_desc : final_ram) {
        const std::size_t& address = memory_desc[0];
        const int& data = memory_desc[1];
        if (memory[address] != data) return false;
    }
    return true;
}