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
#include <iosfwd>
#include <string>
#include <open_bloomrf/storage/storage_layout.hpp>

namespace bloomrf {

    /// A single trace tree covering a fixed-width slice of the key space.
    class TraceLevel {
    public:
        TraceLevel(std::uint8_t height,
                  std::uint8_t hash_functions,
                  StorageAreaType area = StorageAreaType::None);

        [[nodiscard]] constexpr std::uint16_t width() const noexcept {
            return width_;
        }

        [[nodiscard]] constexpr std::uint8_t levels_in_tree() const noexcept {
            return levels_in_tree_;
        }

        /// Usually levels_in_tree + 1: the tree handles its own levels plus
        /// the transition into the next tree/layer.
        [[nodiscard]] constexpr std::uint8_t levels_handled() const noexcept {
            return n_levels_handled_;
        }

        [[nodiscard]] constexpr std::uint8_t hash_functions() const noexcept {
            return hash_functions_;
        }

        [[nodiscard]] constexpr std::uint8_t location_mapper() const noexcept {
            return location_mapper_;
        }

        [[nodiscard]] constexpr std::uint16_t subkey_helper() const noexcept {
            return subkey_helper_;
        }

        [[nodiscard]] constexpr bool is_exact() const noexcept {
            return this->area() == StorageAreaType::Exact;
        }

        [[nodiscard]] constexpr StorageAreaType area() const noexcept {
            return area_;
        }

        [[nodiscard]] std::string to_string() const;

        friend std::ostream& operator<<(std::ostream& os, const TraceLevel& tree) {
            return os << tree.to_string();
        }

    private:
        const std::uint16_t width_;
        const std::uint16_t subkey_helper_;
        const std::uint8_t levels_in_tree_;
        const std::uint8_t n_levels_handled_;
        const std::uint8_t hash_functions_;
        const std::uint8_t location_mapper_;
        const StorageAreaType area_;


        static constexpr std:: uint16_t compute_width(std::uint8_t height) noexcept {
            return static_cast<uint16_t>(1 << (height - 1));
        }
        static constexpr std::uint8_t compute_levels(std::uint8_t height) noexcept {
            return static_cast<uint8_t>(height - 1);
        }
        [[nodiscard]] constexpr std::uint16_t compute_subkey_helper() const noexcept;
        [[nodiscard]] constexpr std::uint8_t compute_location_mapper() const noexcept;

    };
}
