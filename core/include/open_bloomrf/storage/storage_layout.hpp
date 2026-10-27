//
// Created by crizz on 21.10.26.
//
/**
* Copyright (c) 2026 cR!zZ.
 * All rights reserved.
 *
 * This project is not licensed for redistribution or commercial use.
 *
 */

#pragma once

#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

namespace bloomrf {

    enum class StorageAreaType : std::uint8_t {
        None = 0,
        Exact = 1,
        Mid = 2,
        Tail = 3,
    };

    class StorageRegion {
    private:
        const std::uint32_t byte_offset_;
        const std::uint32_t byte_length_;
        const std::uint32_t end_byte_;

    public:
        StorageRegion(const std::uint32_t offset = 0, const std::uint32_t length = 0) noexcept
        : byte_offset_{offset}, byte_length_{length}, end_byte_{offset + length} {}

        [[nodiscard]] constexpr std::uint32_t byte_offset() const noexcept { return byte_offset_; }
        [[nodiscard]] constexpr std::uint32_t byte_length() const noexcept { return byte_length_; }
        [[nodiscard]] constexpr std::uint32_t end_byte() const noexcept { return end_byte_; }

        [[nodiscard]] constexpr bool contains(const std::uint32_t byte_offset) const noexcept {
            return byte_offset >= byte_offset_ && byte_offset < end_byte_;
        }

    };


    class StorageLayout {
    public:
        static StorageLayout flat(std::uint32_t filter_size_bytes);

        static StorageLayout segmented(
            std::uint32_t filter_size_bytes,
            std::uint32_t exact_bytes,
            std::uint32_t middle_bytes);

        [[nodiscard]] StorageRegion region_for(StorageAreaType type) const;

        [[nodiscard]] constexpr std::uint32_t filter_size_bytes() const noexcept {
            return filter_size_bytes_;
        }

        [[nodiscard]] constexpr bool is_segmented() const noexcept {
            return mode_ == Mode::Segmented;
        }

        [[nodiscard]] static constexpr std::array<StorageAreaType, 3> segmented_areas() {
            return {StorageAreaType::Exact, StorageAreaType::Mid, StorageAreaType::Tail};
        }


    private:

        enum class Mode : std::uint8_t {
            Flat,
            Segmented,
        };
        constexpr StorageLayout(
            Mode mode,
            std::uint32_t filter_size_bytes,
            StorageRegion whole_filter,
            StorageRegion exact = {},
            StorageRegion mid = {},
            StorageRegion tail = {}) noexcept;

        constexpr void require_segmented() const {
            if (mode_ != Mode::Segmented) {
                throw std::invalid_argument(
                    "storage area is not available in flat layout");
            }
        }

        Mode mode_;
        std::uint32_t filter_size_bytes_;

        StorageRegion whole_filter_;
        StorageRegion exact_;
        StorageRegion mid_;
        StorageRegion tail_;
    };
}
