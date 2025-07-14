// STL headers
#include <string>

// Local headers
#include "bus.hpp"

Bus::Bus()
{
    ram = &addDevice<Memory>();
    cpu = &addDevice<CPU>(*this);
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
