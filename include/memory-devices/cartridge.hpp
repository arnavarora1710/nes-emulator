#pragma once

#include <lexy/dsl.hpp>
#include <string>
#include <vector>

#include "device.hpp"

using byte_t = std::uint8_t;

struct ROM_Metadata {
    byte_t prg_pages, chr_pages;
    byte_t flags6, flags7, flags8, flags9, flags10;
};

namespace iNES {
    namespace dsl = lexy::dsl;

    struct Header {
        static constexpr byte_t MAGIC_NUMBER = 0x1A;
        static constexpr auto rule = dsl::lit_b<'N'> + dsl::lit_b<'E'> +
                                     dsl::lit_b<'S'> + dsl::lit_b<MAGIC_NUMBER>;
        static constexpr auto value = lexy::noop;
    };

    struct Meta_Parser {
        static constexpr auto NUM_META_BYTES = 7;
        static constexpr auto rule = dsl::p<Header> + dsl::times<NUM_META_BYTES>(dsl::bint8) + dsl::any;
        static constexpr auto value = lexy::construct<ROM_Metadata>;
    };
};

class Cartridge final : public Device {
public:
    static constexpr std::size_t HEADER_SIZE = 16; // 16 byte header
    static constexpr std::size_t PRG_BANK_SIZE = 16 * 1024; // 16KB per bank
    static constexpr std::size_t CHR_BANK_SIZE = 8 * 1024; // 8KB per bank

    explicit Cartridge(const std::string& file_path);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t data) override;

    [[nodiscard]] bool isAddressInRange(uint16_t address) const override;

    [[nodiscard]] byte_t getMapperNumber() const;
private:
    ROM_Metadata metadata{};
    std::vector<byte_t> cartridge{}, prg{}, chr{};
};