#pragma once

// STL headers
#include <memory>
#include <string>
#include <string_view>
#include <vector>

// Device includes
#include "cartridge.hpp"
#include "device.hpp"
#include "memory.hpp"

class Bus {
public:
    using DevicePtr = std::shared_ptr<Device>;

    class BusError final : public std::runtime_error {
    public:
        explicit BusError(const std::string_view message)
            : std::runtime_error(std::string(message)) {}
    };

    Bus() = default;
    ~Bus() = default;

    // Device management
    template <typename DeviceType, typename... Args>
    DeviceType &addDevice(Args &&...args) {
        auto device = std::make_shared<DeviceType>(std::forward<Args>(args)...);
        devices.push_back(device);
        return *device;
    }

    // Generic device access by type
    template <typename DeviceType>
    DeviceType *getDevice() const {
        for (const auto &device : devices) {
            if (auto typed_device = dynamic_cast<DeviceType *>(device.get())) {
                return typed_device;
            }
        }
        return nullptr;
    }

    // Core bus operations
    [[nodiscard]] uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t data) const;
    void insertCartridge(const std::string& file_path);

    // Convenient access to common devices
    [[nodiscard]] Memory &getRAM() const;
    [[nodiscard]] Cartridge &getCartridge() const;

protected:
    std::vector<DevicePtr> devices;

    // Quick access to commonly used devices
    Memory *ram;
    Cartridge *cartridge;

    // Helper method to find a device for address
    [[nodiscard]] Device *findDeviceForAddress(uint16_t address) const;
};
