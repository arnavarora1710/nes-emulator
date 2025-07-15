// STL headers
#include <string>

// Local headers
#include "bus.hpp"

Bus::Bus()
{
    ram = &addDevice<Memory>();
    cpu = &addDevice<CPU>(*this);
}

template <typename DeviceType, typename... Args>
DeviceType &Bus::addDevice(Args &&...args) 
{
    auto device = std::make_shared<DeviceType>(std::forward<Args>(args)...);
    devices.push_back(device);
    return *device;
}

template <typename DeviceType>
DeviceType *Bus::getDevice() const
{
    for (const auto &device : devices)
    {
        if (auto typed_device = dynamic_cast<DeviceType *>(device.get()))
        {
            return typed_device;
        }
    }
    return nullptr;
}

uint8_t Bus::read(uint16_t address)
{
    Device *device = findDeviceForAddress(address);
    if (device)
    {
        return device->read(address);
    }
    throw BusError("No device found for read at address: 0x" +
                   std::to_string(address));
}

void Bus::write(uint16_t address, uint8_t data)
{
    Device *device = findDeviceForAddress(address);
    if (device)
    {
        device->write(address, data);
        return;
    }
    throw BusError("No device found for write at address: 0x" +
                   std::to_string(address));
}

Device *Bus::findDeviceForAddress(uint16_t address) const
{
    for (const auto &device : devices)
    {
        if (device->isAddressInRange(address))
        {
            return device.get();
        }
    }
    return nullptr;
}

CPU &Bus::getCPU() const
{
    if (cpu)
    {
        return *cpu;
    }
    throw BusError("CPU not found on the bus");
}

Memory &Bus::getRAM() const
{
    if (ram)
    {
        return *ram;
    }
    throw BusError("RAM not found on the bus");
}
