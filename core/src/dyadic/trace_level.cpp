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

#include <open_bloomrf/dyadic/trace_level.hpp>

#include <iostream>
#include <stdexcept>



namespace bloomrf {

    constexpr std::uint16_t TraceLevel::compute_subkey_helper() const noexcept {
        return static_cast<uint16_t>(this->width_ - 1);
    }
    constexpr std::uint8_t TraceLevel::compute_location_mapper() const noexcept {
        return static_cast<uint8_t>(this->width_ < 8 ? 0 :  (8 * sizeof(unsigned long long)
            - std::countl_zero(static_cast<unsigned long long>(this->width_ >> 3)) - 1));
    }

/// A single trace tree covering a fixed-width slice of the key space.
    TraceLevel::TraceLevel(std::uint8_t height,
              const std::uint8_t hash_functions,
              const StorageAreaType area)
        : width_{compute_width(height)},
          subkey_helper_{compute_subkey_helper()},
          levels_in_tree_{compute_levels(height)},
          n_levels_handled_{height},
          hash_functions_{hash_functions},
          location_mapper_{compute_location_mapper()},
          area_{area}
    {
        if (height < 2 || height > 10) {
            if (area != StorageAreaType::Exact || height < 2)
                throw std::invalid_argument("TraceTree height must be in [1, 10]");
        }
        if (hash_functions == 0) {
            throw std::invalid_argument("TraceTree hash_functions must be greater than 0.");
        }
        switch (area) {
            case StorageAreaType::None:
            case StorageAreaType::Exact:
            case StorageAreaType::Mid:
            case StorageAreaType::Tail:
                break;
            default:
                throw std::invalid_argument("TraceTree area type must of type StorageAreaType.");
        }
    }



    std::string TraceLevel::to_string() const {
        return std::format(
            "TraceTree:\n"
            "  width:            {}\n"
            "  levels_in_tree:   {}\n"
            "  levels_handled:   {}\n"
            "  hash_functions:   {}\n"
            "  location_mapper:  {}\n"
            "  subkey_helper:    {}\n"
            "  exact_storage:    {}\n"
            "  area:             {}\n",
            this->width(),
            this->levels_in_tree(),
            this->levels_handled(),
            this->hash_functions(),
            this->location_mapper(),
            this->subkey_helper(),
            this->is_exact(),
            static_cast<uint8_t>(this->area())
        );
    }

} // namespace bloomrf