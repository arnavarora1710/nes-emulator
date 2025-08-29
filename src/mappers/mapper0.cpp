#include "mapper.hpp"

class Mapper0 final : public Mapper {
public:
    explicit Mapper0(const ROM_Metadata& rom_meta, CPUBus& cpu_bus, PPUBus& ppu_bus)
                    : Mapper(rom_meta, cpu_bus, ppu_bus)
    {
        _alias = "mapper000";
    }
};