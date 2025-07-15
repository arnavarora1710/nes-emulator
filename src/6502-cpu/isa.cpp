#include "isa.hpp"

namespace AddressingMode
{
	uint8_t IMP() { return 0; }
	uint8_t IMM() { return 0; }
	uint8_t ZP0() { return 0; }
	uint8_t ZPX() { return 0; }
	uint8_t ZPY() { return 0; }
	uint8_t REL() { return 0; }
	uint8_t ABS() { return 0; }
	uint8_t ABX() { return 0; }
	uint8_t ABY() { return 0; }
	uint8_t IND() { return 0; }
	uint8_t IZX() { return 0; }
	uint8_t IZY() { return 0; }
};

namespace Instructions
{
    uint8_t ADC() { return 0; } uint8_t AND() { return 0; } uint8_t ASL() { return 0; } uint8_t BCC() { return 0; }
    uint8_t BCS() { return 0; } uint8_t BEQ() { return 0; } uint8_t BIT() { return 0; } uint8_t BMI() { return 0; }
    uint8_t BNE() { return 0; } uint8_t BPL() { return 0; } uint8_t BRK() { return 0; } uint8_t BVC() { return 0; }
    uint8_t BVS() { return 0; } uint8_t CLC() { return 0; } uint8_t CLD() { return 0; } uint8_t CLI() { return 0; }
    uint8_t CLV() { return 0; } uint8_t CMP() { return 0; } uint8_t CPX() { return 0; } uint8_t CPY() { return 0; }
    uint8_t DEC() { return 0; } uint8_t DEX() { return 0; } uint8_t DEY() { return 0; } uint8_t EOR() { return 0; }
    uint8_t INC() { return 0; } uint8_t INX() { return 0; } uint8_t INY() { return 0; } uint8_t JMP() { return 0; }
    uint8_t JSR() { return 0; } uint8_t LDA() { return 0; } uint8_t LDX() { return 0; } uint8_t LDY() { return 0; }
    uint8_t LSR() { return 0; } uint8_t NOP() { return 0; } uint8_t ORA() { return 0; } uint8_t PHA() { return 0; }
    uint8_t PHP() { return 0; } uint8_t PLA() { return 0; } uint8_t PLP() { return 0; } uint8_t ROL() { return 0; }
    uint8_t ROR() { return 0; } uint8_t RTI() { return 0; } uint8_t RTS() { return 0; } uint8_t SBC() { return 0; }
    uint8_t SEC() { return 0; } uint8_t SED() { return 0; } uint8_t SEI() { return 0; } uint8_t STA() { return 0; }
    uint8_t STX() { return 0; } uint8_t STY() { return 0; } uint8_t TAX() { return 0; } uint8_t TAY() { return 0; }
    uint8_t TSX() { return 0; } uint8_t TXA() { return 0; } uint8_t TXS() { return 0; } uint8_t TYA() { return 0; }

    uint8_t XXX() { return 0; }
};

namespace a = AddressingMode;
namespace i = Instructions;

ISA::ISA()
{
	instructions = {
		{"BRK", &i::BRK, &a::IMM, 7},		{"ORA", &i::ORA, &a::IZX, 6},		{"???", &i::XXX, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 8},		{"???", &i::NOP, &a::IMP, 3},		{"ORA", &i::ORA, &a::ZP0, 3},
		{"ASL", &i::ASL, &a::ZP0, 5},		{"???", &i::XXX, &a::IMP, 5},		{"PHP", &i::PHP, &a::IMP, 3},
		{"ORA", &i::ORA, &a::IMM, 2},		{"ASL", &i::ASL, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 2},
		{"???", &i::NOP, &a::IMP, 4},		{"ORA", &i::ORA, &a::ABS, 4},		{"ASL", &i::ASL, &a::ABS, 6},
		{"???", &i::XXX, &a::IMP, 6},		{"BPL", &i::BPL, &a::REL, 2},		{"ORA", &i::ORA, &a::IZY, 5},
		{"???", &i::XXX, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 8},		{"???", &i::NOP, &a::IMP, 4},
		{"ORA", &i::ORA, &a::ZPX, 4},		{"ASL", &i::ASL, &a::ZPX, 6},		{"???", &i::XXX, &a::IMP, 6},
		{"CLC", &i::CLC, &a::IMP, 2},		{"ORA", &i::ORA, &a::ABY, 4},		{"???", &i::NOP, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 7},		{"???", &i::NOP, &a::IMP, 4},		{"ORA", &i::ORA, &a::ABX, 4},
		{"ASL", &i::ASL, &a::ABX, 7},		{"???", &i::XXX, &a::IMP, 7},		{"JSR", &i::JSR, &a::ABS, 6},
		{"AND", &i::AND, &a::IZX, 6},		{"???", &i::XXX, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 8},
		{"BIT", &i::BIT, &a::ZP0, 3},		{"AND", &i::AND, &a::ZP0, 3},		{"ROL", &i::ROL, &a::ZP0, 5},
		{"???", &i::XXX, &a::IMP, 5},		{"PLP", &i::PLP, &a::IMP, 4},		{"AND", &i::AND, &a::IMM, 2},
		{"ROL", &i::ROL, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 2},		{"BIT", &i::BIT, &a::ABS, 4},
		{"AND", &i::AND, &a::ABS, 4},		{"ROL", &i::ROL, &a::ABS, 6},		{"???", &i::XXX, &a::IMP, 6},
		{"BMI", &i::BMI, &a::REL, 2},		{"AND", &i::AND, &a::IZY, 5},		{"???", &i::XXX, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 8},		{"???", &i::NOP, &a::IMP, 4},		{"AND", &i::AND, &a::ZPX, 4},
		{"ROL", &i::ROL, &a::ZPX, 6},		{"???", &i::XXX, &a::IMP, 6},		{"SEC", &i::SEC, &a::IMP, 2},
		{"AND", &i::AND, &a::ABY, 4},		{"???", &i::NOP, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 7},
		{"???", &i::NOP, &a::IMP, 4},		{"AND", &i::AND, &a::ABX, 4},		{"ROL", &i::ROL, &a::ABX, 7},
		{"???", &i::XXX, &a::IMP, 7},		{"RTI", &i::RTI, &a::IMP, 6},		{"EOR", &i::EOR, &a::IZX, 6},
		{"???", &i::XXX, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 8},		{"???", &i::NOP, &a::IMP, 3},
		{"EOR", &i::EOR, &a::ZP0, 3},		{"LSR", &i::LSR, &a::ZP0, 5},		{"???", &i::XXX, &a::IMP, 5},
		{"PHA", &i::PHA, &a::IMP, 3},		{"EOR", &i::EOR, &a::IMM, 2},		{"LSR", &i::LSR, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 2},		{"JMP", &i::JMP, &a::ABS, 3},		{"EOR", &i::EOR, &a::ABS, 4},
		{"LSR", &i::LSR, &a::ABS, 6},		{"???", &i::XXX, &a::IMP, 6},		{"BVC", &i::BVC, &a::REL, 2},
		{"EOR", &i::EOR, &a::IZY, 5},		{"???", &i::XXX, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 8},
		{"???", &i::NOP, &a::IMP, 4},		{"EOR", &i::EOR, &a::ZPX, 4},		{"LSR", &i::LSR, &a::ZPX, 6},
		{"???", &i::XXX, &a::IMP, 6},		{"CLI", &i::CLI, &a::IMP, 2},		{"EOR", &i::EOR, &a::ABY, 4},
		{"???", &i::NOP, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 7},		{"???", &i::NOP, &a::IMP, 4},
		{"EOR", &i::EOR, &a::ABX, 4},		{"LSR", &i::LSR, &a::ABX, 7},		{"???", &i::XXX, &a::IMP, 7},
		{"RTS", &i::RTS, &a::IMP, 6},		{"ADC", &i::ADC, &a::IZX, 6},		{"???", &i::XXX, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 8},		{"???", &i::NOP, &a::IMP, 3},		{"ADC", &i::ADC, &a::ZP0, 3},
		{"ROR", &i::ROR, &a::ZP0, 5},		{"???", &i::XXX, &a::IMP, 5},		{"PLA", &i::PLA, &a::IMP, 4},
		{"ADC", &i::ADC, &a::IMM, 2},		{"ROR", &i::ROR, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 2},
		{"JMP", &i::JMP, &a::IND, 5},		{"ADC", &i::ADC, &a::ABS, 4},		{"ROR", &i::ROR, &a::ABS, 6},
		{"???", &i::XXX, &a::IMP, 6},		{"BVS", &i::BVS, &a::REL, 2},		{"ADC", &i::ADC, &a::IZY, 5},
		{"???", &i::XXX, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 8},		{"???", &i::NOP, &a::IMP, 4},
		{"ADC", &i::ADC, &a::ZPX, 4},		{"ROR", &i::ROR, &a::ZPX, 6},		{"???", &i::XXX, &a::IMP, 6},
		{"SEI", &i::SEI, &a::IMP, 2},		{"ADC", &i::ADC, &a::ABY, 4},		{"???", &i::NOP, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 7},		{"???", &i::NOP, &a::IMP, 4},		{"ADC", &i::ADC, &a::ABX, 4},
		{"ROR", &i::ROR, &a::ABX, 7},		{"???", &i::XXX, &a::IMP, 7},		{"???", &i::NOP, &a::IMP, 2},
		{"STA", &i::STA, &a::IZX, 6},		{"???", &i::NOP, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 6},
		{"STY", &i::STY, &a::ZP0, 3},		{"STA", &i::STA, &a::ZP0, 3},		{"STX", &i::STX, &a::ZP0, 3},
		{"???", &i::XXX, &a::IMP, 3},		{"DEY", &i::DEY, &a::IMP, 2},		{"???", &i::NOP, &a::IMP, 2},
		{"TXA", &i::TXA, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 2},		{"STY", &i::STY, &a::ABS, 4},
		{"STA", &i::STA, &a::ABS, 4},		{"STX", &i::STX, &a::ABS, 4},		{"???", &i::XXX, &a::IMP, 4},
		{"BCC", &i::BCC, &a::REL, 2},		{"STA", &i::STA, &a::IZY, 6},		{"???", &i::XXX, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 6},		{"STY", &i::STY, &a::ZPX, 4},		{"STA", &i::STA, &a::ZPX, 4},
		{"STX", &i::STX, &a::ZPY, 4},		{"???", &i::XXX, &a::IMP, 4},		{"TYA", &i::TYA, &a::IMP, 2},
		{"STA", &i::STA, &a::ABY, 5},		{"TXS", &i::TXS, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 5},
		{"???", &i::NOP, &a::IMP, 5},		{"STA", &i::STA, &a::ABX, 5},		{"???", &i::XXX, &a::IMP, 5},
		{"???", &i::XXX, &a::IMP, 5},		{"LDY", &i::LDY, &a::IMM, 2},		{"LDA", &i::LDA, &a::IZX, 6},
		{"LDX", &i::LDX, &a::IMM, 2},		{"???", &i::XXX, &a::IMP, 6},		{"LDY", &i::LDY, &a::ZP0, 3},
		{"LDA", &i::LDA, &a::ZP0, 3},		{"LDX", &i::LDX, &a::ZP0, 3},		{"???", &i::XXX, &a::IMP, 3},
		{"TAY", &i::TAY, &a::IMP, 2},		{"LDA", &i::LDA, &a::IMM, 2},		{"TAX", &i::TAX, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 2},		{"LDY", &i::LDY, &a::ABS, 4},		{"LDA", &i::LDA, &a::ABS, 4},
		{"LDX", &i::LDX, &a::ABS, 4},		{"???", &i::XXX, &a::IMP, 4},		{"BCS", &i::BCS, &a::REL, 2},
		{"LDA", &i::LDA, &a::IZY, 5},		{"???", &i::XXX, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 5},
		{"LDY", &i::LDY, &a::ZPX, 4},		{"LDA", &i::LDA, &a::ZPX, 4},		{"LDX", &i::LDX, &a::ZPY, 4},
		{"???", &i::XXX, &a::IMP, 4},		{"CLV", &i::CLV, &a::IMP, 2},		{"LDA", &i::LDA, &a::ABY, 4},
		{"TSX", &i::TSX, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 4},		{"LDY", &i::LDY, &a::ABX, 4},
		{"LDA", &i::LDA, &a::ABX, 4},		{"LDX", &i::LDX, &a::ABY, 4},		{"???", &i::XXX, &a::IMP, 4},
		{"CPY", &i::CPY, &a::IMM, 2},		{"CMP", &i::CMP, &a::IZX, 6},		{"???", &i::NOP, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 8},		{"CPY", &i::CPY, &a::ZP0, 3},		{"CMP", &i::CMP, &a::ZP0, 3},
		{"DEC", &i::DEC, &a::ZP0, 5},		{"???", &i::XXX, &a::IMP, 5},		{"INY", &i::INY, &a::IMP, 2},
		{"CMP", &i::CMP, &a::IMM, 2},		{"DEX", &i::DEX, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 2},
		{"CPY", &i::CPY, &a::ABS, 4},		{"CMP", &i::CMP, &a::ABS, 4},		{"DEC", &i::DEC, &a::ABS, 6},
		{"???", &i::XXX, &a::IMP, 6},		{"BNE", &i::BNE, &a::REL, 2},		{"CMP", &i::CMP, &a::IZY, 5},
		{"???", &i::XXX, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 8},		{"???", &i::NOP, &a::IMP, 4},
		{"CMP", &i::CMP, &a::ZPX, 4},		{"DEC", &i::DEC, &a::ZPX, 6},		{"???", &i::XXX, &a::IMP, 6},
		{"CLD", &i::CLD, &a::IMP, 2},		{"CMP", &i::CMP, &a::ABY, 4},		{"NOP", &i::NOP, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 7},		{"???", &i::NOP, &a::IMP, 4},		{"CMP", &i::CMP, &a::ABX, 4},
		{"DEC", &i::DEC, &a::ABX, 7},		{"???", &i::XXX, &a::IMP, 7},		{"CPX", &i::CPX, &a::IMM, 2},
		{"SBC", &i::SBC, &a::IZX, 6},		{"???", &i::NOP, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 8},
		{"CPX", &i::CPX, &a::ZP0, 3},		{"SBC", &i::SBC, &a::ZP0, 3},		{"INC", &i::INC, &a::ZP0, 5},
		{"???", &i::XXX, &a::IMP, 5},		{"INX", &i::INX, &a::IMP, 2},		{"SBC", &i::SBC, &a::IMM, 2},
		{"NOP", &i::NOP, &a::IMP, 2},		{"???", &i::SBC, &a::IMP, 2},		{"CPX", &i::CPX, &a::ABS, 4},
		{"SBC", &i::SBC, &a::ABS, 4},		{"INC", &i::INC, &a::ABS, 6},		{"???", &i::XXX, &a::IMP, 6},
		{"BEQ", &i::BEQ, &a::REL, 2},		{"SBC", &i::SBC, &a::IZY, 5},		{"???", &i::XXX, &a::IMP, 2},
		{"???", &i::XXX, &a::IMP, 8},		{"???", &i::NOP, &a::IMP, 4},		{"SBC", &i::SBC, &a::ZPX, 4},
		{"INC", &i::INC, &a::ZPX, 6},		{"???", &i::XXX, &a::IMP, 6},		{"SED", &i::SED, &a::IMP, 2},
		{"SBC", &i::SBC, &a::ABY, 4},		{"NOP", &i::NOP, &a::IMP, 2},		{"???", &i::XXX, &a::IMP, 7},
		{"???", &i::NOP, &a::IMP, 4},		{"SBC", &i::SBC, &a::ABX, 4},		{"INC", &i::INC, &a::ABX, 7},
		{"???", &i::XXX, &a::IMP, 7},
	};
}

uint8_t ISA::execute(uint8_t opcode) const
{
	const Instruction &instr = instructions[opcode];
	uint8_t cur_cycles = instr.cycles;
	uint8_t additional_cycles_1 = instr.addrMode();
	uint8_t additional_cycles_2 = instr.operation();
	cur_cycles += (additional_cycles_1 & additional_cycles_2);
	return cur_cycles;
}
