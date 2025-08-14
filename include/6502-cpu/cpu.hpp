#pragma once

#include "device.hpp"
#include "isa.hpp"
#include "json.hpp"

#include <map>

using json = nlohmann::json;

// forward declaration
class Bus;

class CPU final : public Device {
public:
    using Register = uint8_t;
    using ProgramCounter = uint16_t;

    enum class StatusBit {
        CARRY_BIT = (1 << 0),                 // Carry flag
        ZERO_BIT = (1 << 1),                  // Zero flag
        INTERRUPT_DISABLE_BIT = (1 << 2),     // Interrupt disable flag
        DECIMAL_BIT = (1 << 3),               // Decimal mode flag
        BREAK_CMD_BIT = (1 << 4),             // Break command flag
        UNUSED_BIT = (1 << 5),                // Unused flag
        OVERFLOW_BIT = (1 << 6),              // Overflow flag
        SIGN_BIT = (1 << 7),                  // Sign flag (negative)
    };

    const std::map<StatusBit, std::string> status_bit_names = {
        { StatusBit::CARRY_BIT,             "CARRY_BIT" },
        { StatusBit::ZERO_BIT,              "ZERO_BIT" },
        { StatusBit::INTERRUPT_DISABLE_BIT, "INTERRUPT_DISABLE_BIT" },
        { StatusBit::DECIMAL_BIT,           "DECIMAL_BIT" },
        { StatusBit::BREAK_CMD_BIT,         "BREAK_CMD_BIT" },
        { StatusBit::UNUSED_BIT,            "UNUSED_BIT" },
        { StatusBit::OVERFLOW_BIT,          "OVERFLOW_BIT" },
        { StatusBit::SIGN_BIT,              "SIGN_BIT" }
    };

    static constexpr uint16_t STK_PTR_OFFSET = 0x0100;

    // constructors and destructors
    // delete the default constructor to ensure CPU is always created with a Bus reference
    CPU() = delete;
    explicit CPU(Bus &bus) : m_bus(bus), m_isa(*this) {
        reset();
    }
    ~CPU() override = default;

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

    // No device can command the CPU to read or write directly.
    [[nodiscard]] bool isAddressInRange(uint16_t address) const override { return false; }

    // CPU operations
    uint8_t fetch();
    void clock();
    void reset();
    void interrupt();
    void nmi();
    void print_cpu_state() const;
    void init_with_rom(const json& rom_test);
    bool check_final_state(const json& rom_test);
    [[nodiscard]] std::string get_instr_name(uint8_t opcode) const;
    [[nodiscard]] uint8_t get_cycles() const {
        return m_cpuState.cycles;
    }
    [[nodiscard]] uint8_t getFlag(StatusBit bit) const;

private:
    // Reference to the bus for communication with other devices
    Bus &m_bus;

    // CPU registers
    Registers m_registers;

    // CPU state
    CPUState m_cpuState;

    // instruction set architecture (ISA) for the CPU
    ISA m_isa;

    void setFlag(StatusBit bit, bool value);

    friend struct ISA;
};
