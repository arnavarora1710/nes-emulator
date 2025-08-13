#include "bus.hpp"

int main() {
    Bus bus{};
    Memory& memory = bus.getRAM();
    memory.write(0x0000, 0x0069);
    memory.write(0x0001, 69);
    CPU& cpu = bus.getCPU();
    cpu.clock();
}
