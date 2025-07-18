#include "isa.hpp"
#include "cpu.hpp"

#define OP(op) &ISA::op
#define AM(am) &ISA::am

ISA::ISA(CPU& cpu) : m_cpu(cpu), m_registers(cpu.m_registers), m_cpuState(cpu.m_cpuState)
{
    m_instructions = {
        {"BRK", OP(BRK), AM(IMM), 7},        {"ORA", OP(ORA), AM(IZX), 6},        {"???", OP(XXX), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 8},        {"???", OP(NOP), AM(IMP), 3},        {"ORA", OP(ORA), AM(ZP0), 3},
        {"ASL", OP(ASL), AM(ZP0), 5},        {"???", OP(XXX), AM(IMP), 5},        {"PHP", OP(PHP), AM(IMP), 3},
        {"ORA", OP(ORA), AM(IMM), 2},        {"ASL", OP(ASL), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 2},
        {"???", OP(NOP), AM(IMP), 4},        {"ORA", OP(ORA), AM(ABS), 4},        {"ASL", OP(ASL), AM(ABS), 6},
        {"???", OP(XXX), AM(IMP), 6},        {"BPL", OP(BPL), AM(REL), 2},        {"ORA", OP(ORA), AM(IZY), 5},
        {"???", OP(XXX), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 8},        {"???", OP(NOP), AM(IMP), 4},
        {"ORA", OP(ORA), AM(ZPX), 4},        {"ASL", OP(ASL), AM(ZPX), 6},        {"???", OP(XXX), AM(IMP), 6},
        {"CLC", OP(CLC), AM(IMP), 2},        {"ORA", OP(ORA), AM(ABY), 4},        {"???", OP(NOP), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 7},        {"???", OP(NOP), AM(IMP), 4},        {"ORA", OP(ORA), AM(ABX), 4},
        {"ASL", OP(ASL), AM(ABX), 7},        {"???", OP(XXX), AM(IMP), 7},        {"JSR", OP(JSR), AM(ABS), 6},
        {"AND", OP(AND), AM(IZX), 6},        {"???", OP(XXX), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 8},
        {"BIT", OP(BIT), AM(ZP0), 3},        {"AND", OP(AND), AM(ZP0), 3},        {"ROL", OP(ROL), AM(ZP0), 5},
        {"???", OP(XXX), AM(IMP), 5},        {"PLP", OP(PLP), AM(IMP), 4},        {"AND", OP(AND), AM(IMM), 2},
        {"ROL", OP(ROL), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 2},        {"BIT", OP(BIT), AM(ABS), 4},
        {"AND", OP(AND), AM(ABS), 4},        {"ROL", OP(ROL), AM(ABS), 6},        {"???", OP(XXX), AM(IMP), 6},
        {"BMI", OP(BMI), AM(REL), 2},        {"AND", OP(AND), AM(IZY), 5},        {"???", OP(XXX), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 8},        {"???", OP(NOP), AM(IMP), 4},        {"AND", OP(AND), AM(ZPX), 4},
        {"ROL", OP(ROL), AM(ZPX), 6},        {"???", OP(XXX), AM(IMP), 6},        {"SEC", OP(SEC), AM(IMP), 2},
        {"AND", OP(AND), AM(ABY), 4},        {"???", OP(NOP), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 7},
        {"???", OP(NOP), AM(IMP), 4},        {"AND", OP(AND), AM(ABX), 4},        {"ROL", OP(ROL), AM(ABX), 7},
        {"???", OP(XXX), AM(IMP), 7},        {"RTI", OP(RTI), AM(IMP), 6},        {"EOR", OP(EOR), AM(IZX), 6},
        {"???", OP(XXX), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 8},        {"???", OP(NOP), AM(IMP), 3},
        {"EOR", OP(EOR), AM(ZP0), 3},        {"LSR", OP(LSR), AM(ZP0), 5},        {"???", OP(XXX), AM(IMP), 5},
        {"PHA", OP(PHA), AM(IMP), 3},        {"EOR", OP(EOR), AM(IMM), 2},        {"LSR", OP(LSR), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 2},        {"JMP", OP(JMP), AM(ABS), 3},        {"EOR", OP(EOR), AM(ABS), 4},
        {"LSR", OP(LSR), AM(ABS), 6},        {"???", OP(XXX), AM(IMP), 6},        {"BVC", OP(BVC), AM(REL), 2},
        {"EOR", OP(EOR), AM(IZY), 5},        {"???", OP(XXX), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 8},
        {"???", OP(NOP), AM(IMP), 4},        {"EOR", OP(EOR), AM(ZPX), 4},        {"LSR", OP(LSR), AM(ZPX), 6},
        {"???", OP(XXX), AM(IMP), 6},        {"CLI", OP(CLI), AM(IMP), 2},        {"EOR", OP(EOR), AM(ABY), 4},
        {"???", OP(NOP), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 7},        {"???", OP(NOP), AM(IMP), 4},
        {"EOR", OP(EOR), AM(ABX), 4},        {"LSR", OP(LSR), AM(ABX), 7},        {"???", OP(XXX), AM(IMP), 7},
        {"RTS", OP(RTS), AM(IMP), 6},        {"ADC", OP(ADC), AM(IZX), 6},        {"???", OP(XXX), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 8},        {"???", OP(NOP), AM(IMP), 3},        {"ADC", OP(ADC), AM(ZP0), 3},
        {"ROR", OP(ROR), AM(ZP0), 5},        {"???", OP(XXX), AM(IMP), 5},        {"PLA", OP(PLA), AM(IMP), 4},
        {"ADC", OP(ADC), AM(IMM), 2},        {"ROR", OP(ROR), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 2},
        {"JMP", OP(JMP), AM(IND), 5},        {"ADC", OP(ADC), AM(ABS), 4},        {"ROR", OP(ROR), AM(ABS), 6},
        {"???", OP(XXX), AM(IMP), 6},        {"BVS", OP(BVS), AM(REL), 2},        {"ADC", OP(ADC), AM(IZY), 5},
        {"???", OP(XXX), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 8},        {"???", OP(NOP), AM(IMP), 4},
        {"ADC", OP(ADC), AM(ZPX), 4},        {"ROR", OP(ROR), AM(ZPX), 6},        {"???", OP(XXX), AM(IMP), 6},
        {"SEI", OP(SEI), AM(IMP), 2},        {"ADC", OP(ADC), AM(ABY), 4},        {"???", OP(NOP), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 7},        {"???", OP(NOP), AM(IMP), 4},        {"ADC", OP(ADC), AM(ABX), 4},
        {"ROR", OP(ROR), AM(ABX), 7},        {"???", OP(XXX), AM(IMP), 7},        {"???", OP(NOP), AM(IMP), 2},
        {"STA", OP(STA), AM(IZX), 6},        {"???", OP(NOP), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 6},
        {"STY", OP(STY), AM(ZP0), 3},        {"STA", OP(STA), AM(ZP0), 3},        {"STX", OP(STX), AM(ZP0), 3},
        {"???", OP(XXX), AM(IMP), 3},        {"DEY", OP(DEY), AM(IMP), 2},        {"???", OP(NOP), AM(IMP), 2},
        {"TXA", OP(TXA), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 2},        {"STY", OP(STY), AM(ABS), 4},
        {"STA", OP(STA), AM(ABS), 4},        {"STX", OP(STX), AM(ABS), 4},        {"???", OP(XXX), AM(IMP), 4},
        {"BCC", OP(BCC), AM(REL), 2},        {"STA", OP(STA), AM(IZY), 6},        {"???", OP(XXX), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 6},        {"STY", OP(STY), AM(ZPX), 4},        {"STA", OP(STA), AM(ZPX), 4},
        {"STX", OP(STX), AM(ZPY), 4},        {"???", OP(XXX), AM(IMP), 4},        {"TYA", OP(TYA), AM(IMP), 2},
        {"STA", OP(STA), AM(ABY), 5},        {"TXS", OP(TXS), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 5},
        {"???", OP(NOP), AM(IMP), 5},        {"STA", OP(STA), AM(ABX), 5},        {"???", OP(XXX), AM(IMP), 5},
        {"???", OP(XXX), AM(IMP), 5},        {"LDY", OP(LDY), AM(IMM), 2},        {"LDA", OP(LDA), AM(IZX), 6},
        {"LDX", OP(LDX), AM(IMM), 2},        {"???", OP(XXX), AM(IMP), 6},        {"LDY", OP(LDY), AM(ZP0), 3},
        {"LDA", OP(LDA), AM(ZP0), 3},        {"LDX", OP(LDX), AM(ZP0), 3},        {"???", OP(XXX), AM(IMP), 3},
        {"TAY", OP(TAY), AM(IMP), 2},        {"LDA", OP(LDA), AM(IMM), 2},        {"TAX", OP(TAX), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 2},        {"LDY", OP(LDY), AM(ABS), 4},        {"LDA", OP(LDA), AM(ABS), 4},
        {"LDX", OP(LDX), AM(ABS), 4},        {"???", OP(XXX), AM(IMP), 4},        {"BCS", OP(BCS), AM(REL), 2},
        {"LDA", OP(LDA), AM(IZY), 5},        {"???", OP(XXX), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 5},
        {"LDY", OP(LDY), AM(ZPX), 4},        {"LDA", OP(LDA), AM(ZPX), 4},        {"LDX", OP(LDX), AM(ZPY), 4},
        {"???", OP(XXX), AM(IMP), 4},        {"CLV", OP(CLV), AM(IMP), 2},        {"LDA", OP(LDA), AM(ABY), 4},
        {"TSX", OP(TSX), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 4},        {"LDY", OP(LDY), AM(ABX), 4},
        {"LDA", OP(LDA), AM(ABX), 4},        {"LDX", OP(LDX), AM(ABY), 4},        {"???", OP(XXX), AM(IMP), 4},
        {"CPY", OP(CPY), AM(IMM), 2},        {"CMP", OP(CMP), AM(IZX), 6},        {"???", OP(NOP), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 8},        {"CPY", OP(CPY), AM(ZP0), 3},        {"CMP", OP(CMP), AM(ZP0), 3},
        {"DEC", OP(DEC), AM(ZP0), 5},        {"???", OP(XXX), AM(IMP), 5},        {"INY", OP(INY), AM(IMP), 2},
        {"CMP", OP(CMP), AM(IMM), 2},        {"DEX", OP(DEX), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 2},
        {"CPY", OP(CPY), AM(ABS), 4},        {"CMP", OP(CMP), AM(ABS), 4},        {"DEC", OP(DEC), AM(ABS), 6},
        {"???", OP(XXX), AM(IMP), 6},        {"BNE", OP(BNE), AM(REL), 2},        {"CMP", OP(CMP), AM(IZY), 5},
        {"???", OP(XXX), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 8},        {"???", OP(NOP), AM(IMP), 4},
        {"CMP", OP(CMP), AM(ZPX), 4},        {"DEC", OP(DEC), AM(ZPX), 6},        {"???", OP(XXX), AM(IMP), 6},
        {"CLD", OP(CLD), AM(IMP), 2},        {"CMP", OP(CMP), AM(ABY), 4},        {"NOP", OP(NOP), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 7},        {"???", OP(NOP), AM(IMP), 4},        {"CMP", OP(CMP), AM(ABX), 4},
        {"DEC", OP(DEC), AM(ABX), 7},        {"???", OP(XXX), AM(IMP), 7},        {"CPX", OP(CPX), AM(IMM), 2},
        {"SBC", OP(SBC), AM(IZX), 6},        {"???", OP(NOP), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 8},
        {"CPX", OP(CPX), AM(ZP0), 3},        {"SBC", OP(SBC), AM(ZP0), 3},        {"INC", OP(INC), AM(ZP0), 5},
        {"???", OP(XXX), AM(IMP), 5},        {"INX", OP(INX), AM(IMP), 2},        {"SBC", OP(SBC), AM(IMM), 2},
        {"NOP", OP(NOP), AM(IMP), 2},        {"???", OP(SBC), AM(IMP), 2},        {"CPX", OP(CPX), AM(ABS), 4},
        {"SBC", OP(SBC), AM(ABS), 4},        {"INC", OP(INC), AM(ABS), 6},        {"???", OP(XXX), AM(IMP), 6},
        {"BEQ", OP(BEQ), AM(REL), 2},        {"SBC", OP(SBC), AM(IZY), 5},        {"???", OP(XXX), AM(IMP), 2},
        {"???", OP(XXX), AM(IMP), 8},        {"???", OP(NOP), AM(IMP), 4},        {"SBC", OP(SBC), AM(ZPX), 4},
        {"INC", OP(INC), AM(ZPX), 6},        {"???", OP(XXX), AM(IMP), 6},        {"SED", OP(SED), AM(IMP), 2},
        {"SBC", OP(SBC), AM(ABY), 4},        {"NOP", OP(NOP), AM(IMP), 2},        {"???", OP(XXX), AM(IMP), 7},
        {"???", OP(NOP), AM(IMP), 4},        {"SBC", OP(SBC), AM(ABX), 4},        {"INC", OP(INC), AM(ABX), 7},
        {"???", OP(XXX), AM(IMP), 7},
    };
}

ISA::Instruction ISA::getInstruction(uint8_t opcode) const {
    return m_instructions.at(opcode);
}

uint8_t ISA::execute(uint8_t opcode)
{
    const Instruction &instr = m_instructions[opcode];
    uint8_t cur_cycles = instr.cycles;
    const uint8_t additional_cycles_1 = (this->*instr.addrMode)();
    const uint8_t additional_cycles_2 = (this->*instr.operation)();
    cur_cycles += (additional_cycles_1 & additional_cycles_2);
    return cur_cycles;
}
