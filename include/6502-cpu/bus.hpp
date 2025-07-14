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

	// Core bus operations
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t data);

	// Device management
	template <typename DeviceType, typename... Args>
	DeviceType &addDevice(Args &&...args)
	{
		auto device = std::make_shared<DeviceType>(std::forward<Args>(args)...);
		devices.push_back(device);
		return *device;
	}

	// Convenient access to common devices
	CPU &getCPU() const { return *cpu; }
	Memory &getRAM() const { return *ram; }

	// Generic device access by type
	template <typename DeviceType>
	DeviceType *getDevice() const
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

private:
	std::vector<DevicePtr> devices;

	// Quick access to commonly used devices
	CPU *cpu;
	Memory *ram;

	// Helper method to find device for address
	Device *findDeviceForAddress(uint16_t address) const;
};
