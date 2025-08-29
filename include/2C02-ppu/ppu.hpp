#pragma once

#include "device.hpp"

class PPU final : public Device {
public:
    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

    [[nodiscard]] bool isAddressInRange(uint16_t address) const override;
};