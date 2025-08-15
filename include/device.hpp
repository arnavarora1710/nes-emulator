#pragma once

#include <cstdint>

// defines an interface for a device
// example: cpu, ram etc.

class Device {
public:
    virtual ~Device() = default;

    // read from the bus
    virtual uint8_t read(uint16_t address) = 0;
    // write to the bus
    virtual void write(uint16_t address, uint8_t data) = 0;

    // address response range
    [[nodiscard]] virtual bool isAddressInRange(uint16_t address) const = 0;
};
