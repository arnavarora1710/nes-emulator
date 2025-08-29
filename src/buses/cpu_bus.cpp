#include "cpu_bus.hpp"

CPUBus::CPUBus(const bool testing) {
    const std::size_t RAM_SIZE = (testing ? CPU_RAM_SIZE_TEST : CPU_RAM_SIZE);
    const uint16_t MASK = (testing ? CPU_RAM_MASK_TEST : CPU_RAM_MASK);

    ram = &addDevice<Memory>(RAM_SIZE, CPU_RAM_START, MASK);

    cpu = &addDevice<CPU>(*this);
    if (testing)
        cpu->setCycles(0);
}

CPU &CPUBus::getCPU() const
{
    if (cpu)
    {
        return *cpu;
    }
    throw BusError("CPU not found on the bus");
}
