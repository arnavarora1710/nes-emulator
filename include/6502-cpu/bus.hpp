#pragma once

// STL headers
#include <memory>
#include <string>
#include <string_view>
#include <vector>

// Device includes
#include "cpu.hpp"
#include "device.hpp"
#include "memory.hpp"

class Bus {
public:
    using DevicePtr = std::shared_ptr<Device>;

    class BusError : public std::runtime_error
    {
    public:
        explicit BusError(std::string_view message)
            : std::runtime_error(std::string(message)) {}
    };

    Bus();
    ~Bus() = default;

    // Device management
    template <typename DeviceType, typename... Args>
    DeviceType &addDevice(Args &&...args);

    // Generic device access by type
    template <typename DeviceType>
    DeviceType *getDevice() const;

    // Core bus operations
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);

    // Convenient access to common devices
    CPU &getCPU() const;
    Memory &getRAM() const;

private:
    std::vector<DevicePtr> devices;

    // Quick access to commonly used devices
    CPU *cpu;
    Memory *ram;

    // Helper method to find device for address
    Device *findDeviceForAddress(uint16_t address) const;
};
