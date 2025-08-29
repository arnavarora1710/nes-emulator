// STL headers
#include <string>

// Local headers
#include "bus.hpp"

uint8_t Bus::read(const uint16_t address) const {
    if (Device *device = findDeviceForAddress(address)) {
        return device->read(address);
    }
    throw BusError("No device found for read at address: 0x" +
                   std::to_string(address));
}

void Bus::write(const uint16_t address, const uint8_t data) const {
    if (Device *device = findDeviceForAddress(address)) {
        device->write(address, data);
        return;
    }
    throw BusError("No device found for write at address: 0x" +
                   std::to_string(address));
}

void Bus::insertCartridge(const std::string& file_path) {
    cartridge = new Cartridge(file_path);
}

Device *Bus::findDeviceForAddress(uint16_t address) const {
    for (const auto &device : devices) {
        if (device->isAddressInRange(address)) {
            return device.get();
        }
    }
    return nullptr;
}

Memory &Bus::getRAM() const {
    if (ram) {
        return *ram;
    }
    throw BusError("RAM not found on the bus");
}

Cartridge &Bus::getCartridge() const {
    if (cartridge) {
        return *cartridge;
    }
    throw BusError("Cartridge not found on the bus");
}