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

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <atomic>

namespace bloomrf {

    class CachelineBlock {
    public:

        static constexpr std::size_t kBytes = 64; //std::hardware_destructive_interference_size;
        static constexpr std::size_t kWords = kBytes / sizeof(std::uint64_t);
        static constexpr std::size_t kBits = kBytes * 8;
        static_assert(kBytes % sizeof(std::uint64_t) == 0);
        static_assert(kBytes >= 64);

        void or_word(const std::size_t word_index, const std::uint64_t mask) noexcept {
            words_[word_index] |= mask;
        }

        void or_word_concurrent(const std::size_t word_index, const std::uint64_t mask) noexcept {
            std::atomic_ref<std::uint64_t> atomic_word(words_[word_index]);
            (void) atomic_word.fetch_or(mask, std::memory_order_relaxed);
        }

        [[nodiscard]] std::uint64_t match_mask_word(const std::size_t word_index, const std::uint64_t mask) const noexcept {
            return (words_[word_index] & mask);
        }

        [[nodiscard]] std::uint32_t match_count_word(const std::size_t word_index, const std::uint64_t mask) const noexcept{
            return std::popcount(match_mask_word(word_index, mask));
        }

        [[nodiscard]] bool any_word(const std::size_t word_index, const std::uint64_t mask) const noexcept{
            return match_mask_word(word_index, mask) != 0;
        }

        void or_block(const std::span<const std::uint64_t, kWords> mask) noexcept {
#pragma omp simd
            for (std::size_t i = 0; i < kWords; ++i) {
                or_word(i, mask[i]);
            }
        }

        void or_block_concurrent(const std::span<const std::uint64_t, kWords> mask) noexcept {
            for (std::size_t i = 0; i < kWords; ++i) {
                if (mask[i])
                    or_word_concurrent(i, mask[i]);
            }
        }

        [[nodiscard]] std::array<std::uint64_t, kWords> match_mask_block(std::span<const std::uint64_t, kWords> mask) const noexcept {
            std::array<std::uint64_t, kWords> result{};
#pragma omp simd
            for (std::size_t i = 0; i < kWords; ++i) {
                result[i] = match_mask_word(i, mask[i]);
            }
            return result;
        }

        [[nodiscard]] uint32_t match_count_block(const std::span<const std::uint64_t, kWords> mask) const noexcept {
            uint32_t result = 0;
#pragma omp simd reduction(+:result)
            for (std::size_t i = 0; i < kWords; ++i) {
                result += match_count_word(i, mask[i]);
            }
            return result;
        }

        [[nodiscard]] bool any_block(const std::span<const std::uint64_t, kWords> mask) const noexcept {
            std::uint64_t combined = 0;
#pragma omp simd reduction(| : combined)
            for (std::size_t i = 0; i < kWords; ++i) {
                combined |= words_[i] & mask[i];
            }
            return combined != 0;
        }

        [[nodiscard]] std::array<std::uint64_t, kWords> snapshot() const noexcept {
            return words_;
        }

    private:
        alignas(kBytes) std::array<std::uint64_t, kWords> words_{};
    };

}
