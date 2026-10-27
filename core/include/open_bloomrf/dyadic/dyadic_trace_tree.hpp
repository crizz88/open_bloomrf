/**
* Copyright (c) 2026 cR!zZ.
 * All rights reserved.
 *
 * This project is not licensed for redistribution or commercial use.
 *
 */

#pragma once

#include <cstdint>
#include <numeric>
#include <span>
#include <vector>

#include "open_bloomrf/storage/storage_layout.hpp"
#include "trace_level.hpp"

namespace bloomrf {

    class DyadicTraceTree {
    public:
        DyadicTraceTree(
            std::span<const TraceLevel> levels,
            const StorageLayout layout)
            : top_level_{compute_top_level(levels)},
              levels_{levels.begin(), levels.end()},
              layout_{layout} {
            if (levels_.empty()) {
                throw std::invalid_argument("empty level");
            }
        }

        [[nodiscard]] constexpr std::uint8_t top_level() const noexcept {
            return top_level_;
        }

        [[nodiscard]] std::span<const TraceLevel> levels() const noexcept {
            return levels_;
        }

        [[nodiscard]] const TraceLevel& level(std::size_t index) const {
            return levels_.at(index);
        }

        /// Resolve the concrete byte region for a trace level.
        [[nodiscard]] StorageRegion region_for(std::size_t level_index) const {
            const auto& level = levels_.at(level_index);
            return layout_.region_for(level.area());
        }

    private:
        std::uint8_t top_level_;
        std::vector<TraceLevel> levels_;
        StorageLayout layout_;

        [[nodiscard]] static std::uint8_t compute_top_level(std::span<const TraceLevel> levels) {
            const auto total = std::accumulate(levels.begin(),
                levels.end(), std::uint32_t{0},
                [](auto accumulator, auto level) {
                    return accumulator + level.levels_handled();
                });
            if (total > std::numeric_limits<std::uint8_t>::max()) {
                throw std::invalid_argument("trace levels exceed supported dyadic level range");
            }
            return static_cast<std::uint8_t>(total);
        }


    };

} // namespace bloomrf
