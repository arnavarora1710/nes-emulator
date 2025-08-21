#include "cartridge.hpp"

#include <lexy/input/file.hpp>
#include <lexy/action/parse.hpp>
#include <lexy_ext/report_error.hpp>
#include <fstream>

Cartridge::Cartridge(const std::string& file_path) {
    const auto file = lexy::read_file<lexy::byte_encoding>(file_path.c_str());
    if (!file) {
        throw std::runtime_error("Failed to read ROM file");
    }
    const auto& input = file.buffer();
    const auto parsed_meta = lexy::parse<iNES::Meta_Parser>(input, lexy_ext::report_error);
    if (parsed_meta.has_value()) {
        metadata = parsed_meta.value();

        // calculate expected ROM size
        const std::size_t prg_rom_size = metadata.prg_pages * PRG_BANK_SIZE;
        const std::size_t chr_rom_size = metadata.chr_pages * CHR_BANK_SIZE;
        const std::size_t total_rom_size = HEADER_SIZE + prg_rom_size + chr_rom_size;

        std::ifstream rom_file(file_path, std::ios::binary | std::ios::ate);
        assert(rom_file.is_open());

        // calculate actual ROM size
        const std::streamsize rom_size = rom_file.tellg();  // total file size
        assert(rom_size == total_rom_size);
        cartridge.resize(rom_size);

        // read ROM into virtual cartridge
        rom_file.seekg(0);
        rom_file.read(reinterpret_cast<char*>(cartridge.data()), rom_size);
    } else {
        throw std::runtime_error("Failed to parse ROM file");
    }
}