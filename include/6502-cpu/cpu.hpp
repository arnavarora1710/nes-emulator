#pragma once

#include <memory>

#include "device.hpp"
#include "isa.hpp"

// forward declaration
class Bus;

class CPU : public Device {
public:
	using Register = uint8_t;
	using ProgramCounter = uint16_t;

	enum class StatusBit {
		CARRY_BIT = (1 << 0),                 // Carry flag
		ZERO_BIT = (1 << 1),                  // Zero flag
		INTERRUPT_DISABLE_BIT = (1 << 2),     // Interrupt disable flag
		DECIMAL_BIT = (1 << 3),               // Decimal mode flag
		BREAK_CMD_BIT = (1 << 4),             // Break command flag
		OVERFLOW_BIT = (1 << 5),              // Overflow flag
		SIGN_BIT = (1 << 6),                  // Sign flag (negative)
	};

	// constructors and destructors
	// delete the default constructor to ensure CPU is always created with a Bus reference
	CPU() = delete;
	CPU(Bus &bus) : m_bus(bus) {}
	~CPU() = default;

	uint8_t read(uint16_t address) override;

	void write(uint16_t address, uint8_t data) override;

	// No device can command the CPU to read or write directly.
	bool isAddressInRange(uint16_t address) const override { return false; }

	// CPU operations
	void clock();
	void reset();
	void interrupt();
	void nmi();

private:
	// Reference to the bus for communication with other devices
	Bus &m_bus;

	// Registers for CPU state and execution storage
	Register A, X, Y, SP;
	ProgramCounter PC;

	// status byte with 8 bits, each representing a different status flag
	Register Status; 

	// instruction set architecture (ISA) for the CPU
	ISA isa;

	uint8_t getFlag(StatusBit bit) const;

	void setFlag(StatusBit bit, bool value);
};
