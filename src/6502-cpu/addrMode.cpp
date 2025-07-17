#include "isa.hpp"

uint8_t ISA::IMP() { 
    m_cpuState.fetched = m_registers.A;
    return 0; 
}

uint8_t ISA::IMM() { 
    m_cpuState.absAddr = m_registers.PC++;
    return 0; 
}

uint8_t ISA::ZP0() { return 0; }
uint8_t ISA::ZPX() { return 0; }
uint8_t ISA::ZPY() { return 0; }
uint8_t ISA::REL() { return 0; }
uint8_t ISA::ABS() { return 0; }
uint8_t ISA::ABX() { return 0; }
uint8_t ISA::ABY() { return 0; }
uint8_t ISA::IND() { return 0; }
uint8_t ISA::IZX() { return 0; }
uint8_t ISA::IZY() { return 0; }
