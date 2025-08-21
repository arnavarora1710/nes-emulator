#pragma once

#include <array>

#include "device.hpp"
#include "json.hpp"

using json = nlohmann::json;

class Memory final : public Device {
public:
    static constexpr std::size_t MEMORY_SIZE = 64 * 1024; // 64KB
    static constexpr uint16_t MEMORY_START_ADDRESS = 0x0000;
    static constexpr uint16_t MEMORY_END_ADDRESS = MEMORY_START_ADDRESS + MEMORY_SIZE - 1;

    Memory() = default;

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

    [[nodiscard]] bool isAddressInRange(const uint16_t address) const override {
        return MEMORY_START_ADDRESS <= address && address <= MEMORY_END_ADDRESS;
    }

    void loadROM(const json& rom_test);
    bool checkFinalState(const json& rom_test);

private:
    std::array<uint8_t, MEMORY_SIZE> memory{};
};
