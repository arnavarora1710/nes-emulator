#include "bus.hpp"
#include "json.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

using json = nlohmann::json;
namespace fs = std::filesystem;

bool performTest(const json& rom_test, int test_num, bool verbose = false) {
    if (verbose)
        std::cout << rom_test << std::endl;

    const Bus bus{};

    CPU& cpu = bus.getCPU();
    if (cpu.get_instr_name(test_num) == "???") {
        return true;
    }
    cpu.reset();
    cpu.init_with_rom(rom_test);
    if (cpu.getFlag(CPU::StatusBit::DECIMAL_BIT) == true) {
        return true;
    }

    Memory& memory = bus.getRAM();
    memory.load_rom(rom_test);

    const auto& v = rom_test["cycles"];
    do {
        cpu.clock();
    } while (cpu.get_cycles() > 0);
    return cpu.check_final_state(rom_test) and memory.check_final_state(rom_test);
}

int main() {
    for (auto test_num = 0x00; test_num <= 0xff; ++test_num) {
        std::string rom_file = "tests/6502-cpu/";
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(2) << test_num;
        rom_file += ss.str() + ".json";
        const fs::path rom_path = fs::path(PROJECT_ROOT) / rom_file;
        std::ifstream file(rom_path);
        assert(file.is_open());
        json rom_tests;
        file >> rom_tests;

        std::size_t tests_passed = 0, tests_total = rom_tests.size();
        for (const auto& rom_test : rom_tests) {
            bool test_ok = performTest(rom_test, test_num);
            tests_passed += test_ok;
            if (!test_ok) {
                performTest(rom_test, test_num, true);
            }
        }
        std::cout << "Finished testing " << ss.str() << ".json" << std::endl;
        std::cout << "Total tests: " << tests_total << " | Tests Passed: " << tests_passed << std::endl;
    }
}
