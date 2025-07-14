#pragma once

#include <memory>
#include "device.hpp"

// forward declaration
class Bus;

class CPU : public Device {
public:
	// constructors and destructors
	// delete the default constructor to ensure CPU is always created with a Bus reference
	CPU() = delete;
	CPU(Bus &bus) : m_bus(bus) {}
	~CPU() = default;

	// No device can command the CPU to read or write directly.
	bool isAddressInRange(uint16_t address) const override { return false; }

private:
	Bus &m_bus;

	uint8_t read(uint16_t address) override;

	void write(uint16_t address, uint8_t data) override;
};
