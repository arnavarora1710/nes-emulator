#pragma once
#include "cartridge.hpp"
#include "cpu_bus.hpp"
#include "ppu_bus.hpp"

class Mapper {
public:
    Mapper() = delete;
    explicit Mapper(const ROM_Metadata& meta, CPUBus& cpu_bus, PPUBus& ppu_bus);

protected:
    std::string _alias;
    ROM_Metadata _meta;
    CPUBus& _cpu_bus;
    PPUBus& _ppu_bus;
};
