#pragma once

#include "bus.hpp"
#include "ppu.hpp"

class PPUBus final : public Bus {
public:
    static constexpr std::size_t PPU_RAM_SIZE = 2 * 1024; // 2KB
    static constexpr uint16_t PPU_RAM_START = 0x2000;
private:
    PPU& ppu;
};
