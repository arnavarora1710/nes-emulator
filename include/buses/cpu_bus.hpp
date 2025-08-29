#pragma once
#include "cpu.hpp"
#include "bus.hpp"

class CPUBus final : public Bus {
public:
    static constexpr std::size_t CPU_RAM_SIZE = 2 * 1024; // 2KB
    static constexpr uint16_t CPU_RAM_MASK = 0x07FF;

    static constexpr std::size_t CPU_RAM_SIZE_TEST = 64 * 1024; // 64KB
    static constexpr uint16_t CPU_RAM_MASK_TEST = ~0;

    static constexpr uint16_t CPU_RAM_START = 0x0000;

    explicit CPUBus(bool testing);
    [[nodiscard]] CPU &getCPU() const;

private:
    CPU *cpu;
};
