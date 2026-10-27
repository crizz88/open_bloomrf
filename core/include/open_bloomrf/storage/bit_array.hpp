//
// Created by crizz on 26.10.26.
//
/**
* Copyright (c) 2026 cR!zZ.
 * All rights reserved.
 *
 * This project is not licensed for redistribution or commercial use.
 *
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <stdexcept>
#include <vector>

#pragma once

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "cache_line_block.hpp"

namespace bloomrf {

    class BitArray {
    public:
        explicit BitArray(std::uint64_t block_count)
            : blocks_(block_count)
        {
            if (block_count == 0) {
                throw std::invalid_argument("block count must be greater than zero");
            }
        }

        [[nodiscard]] constexpr std::uint64_t block_count() const noexcept {
            return blocks_.size();
        }

        [[nodiscard]] constexpr std::uint64_t size_bytes() const noexcept {
            return blocks_.size() * CachelineBlock::kBytes;
        }

        [[nodiscard]] CachelineBlock& block(std::uint64_t byte_offset) {
            return blocks_.at(byte_offset / CachelineBlock::kBytes);
        }

        [[nodiscard]] const CachelineBlock& block(
            std::uint64_t byte_offset) const
        {
            return blocks_.at(byte_offset / CachelineBlock::kBytes);
        }

    private:
        std::vector<CachelineBlock> blocks_;
    };

}
