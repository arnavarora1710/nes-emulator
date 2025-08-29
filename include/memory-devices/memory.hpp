#pragma once

#include <vector>

#include "device.hpp"
#include "json.hpp"

using json = nlohmann::json;

class Memory final : public Device {
public:
    Memory() = delete;
    Memory(std::size_t size, uint16_t start_addr, uint16_t mirror_mask);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

    [[nodiscard]] bool isAddressInRange(const uint16_t address) const override {
        return memory_start_addr <= address && address <= memory_end_addr;
    }

    void loadROM(const json& rom_test);
    bool checkFinalState(const json& rom_test) const;

private:
    std::vector<uint8_t> memory;
    std::size_t memory_size{};
    uint16_t memory_start_addr{}, memory_end_addr{}, memory_mirror_mask{};
};
