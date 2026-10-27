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
#include <open_bloomrf/storage/storage_layout.hpp>

namespace bloomrf {

    constexpr StorageLayout::StorageLayout(
        const Mode mode,
        std::uint32_t filter_size_bytes,
        const StorageRegion whole_filter,
        const StorageRegion exact,
        const StorageRegion mid,
        const StorageRegion tail) noexcept
        : mode_{mode},
          filter_size_bytes_{filter_size_bytes},
          whole_filter_{whole_filter},
          exact_{exact},
          mid_{mid},
          tail_{tail}
    {}

    StorageLayout StorageLayout::flat(std::uint32_t filter_size_bytes) {
        if (filter_size_bytes == 0) {
            throw std::invalid_argument(
                "filter size must be greater than zero");
        }

        return StorageLayout{
            Mode::Flat,
            filter_size_bytes,
            StorageRegion{0, filter_size_bytes}
        };
    }

    StorageLayout StorageLayout::segmented(
       std::uint32_t filter_size_bytes,
       std::uint32_t exact_bytes,
       std::uint32_t middle_bytes) {
        if (filter_size_bytes == 0) {
            throw std::invalid_argument(
                "filter size must be greater than zero");
        }

        if (exact_bytes + middle_bytes > filter_size_bytes) {
            throw std::invalid_argument(
                "exact and middle segments exceed filter size");
        }

        const auto tail_bytes =
            filter_size_bytes - exact_bytes - middle_bytes;

        return StorageLayout{
            Mode::Segmented,
            filter_size_bytes,
            StorageRegion{0, filter_size_bytes},
            StorageRegion{0, exact_bytes},
            StorageRegion{exact_bytes, middle_bytes},
            StorageRegion{exact_bytes + middle_bytes, tail_bytes}
        };
    }

    [[nodiscard]] StorageRegion StorageLayout::region_for(
    const StorageAreaType type) const {
        switch (type) {
            case StorageAreaType::None:
                return whole_filter_;

            case StorageAreaType::Exact:
                require_segmented();
                return exact_;

            case StorageAreaType::Mid:
                require_segmented();
                return mid_;

            case StorageAreaType::Tail:
                require_segmented();
                return tail_;
        }

        throw std::invalid_argument("unknown storage area type");
    }

}