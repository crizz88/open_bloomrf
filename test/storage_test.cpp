//
// Created by crizz on 26.10.26.
//
#include <ranges>
#include <string>
#include <gtest/gtest.h>

#include <open_bloomrf/storage/bit_array.hpp>
/**
* Copyright (c) 2026 cR!zZ.
 * All rights reserved.
 *
 * This project is not licensed for redistribution or commercial use.
 *
 */

namespace {

    using bloomrf::BitArray;
    using bloomrf::CachelineBlock;

    constexpr std::uint64_t bit(std::uint64_t index) noexcept {
        return std::uint64_t{1} << index;
    }

    /// Bits [from, to)
    constexpr std::uint64_t bits_between(const std::uint64_t from, const std::uint64_t to) noexcept
    {
        const auto lower_mask = (from == 0)
            ? std::uint64_t{0}
        : (bit(from) - 1);

        const auto upper_mask = (to >= 64)
            ? ~std::uint64_t{0}
        : (bit(to) - 1);

        return upper_mask ^ lower_mask;
    }

    TEST(StorageBitArrayTest, Initialization) {
        EXPECT_NO_THROW(BitArray array(16));
        EXPECT_NO_THROW(BitArray bits(16); auto& ret = bits.block(1));
        EXPECT_THROW(BitArray array(0), std::invalid_argument);
    }

    TEST(StorageBitArrayTest, SetAndTestSingleBit) {
        BitArray bits(16);

        auto& block = bits.block(0);

        block.or_word(0, bit(17));
        EXPECT_TRUE(block.any_word(0, bit(17)));
        EXPECT_FALSE(block.any_word(0, bit(16)));
        EXPECT_FALSE(block.any_word(1, bit(17)));

        EXPECT_EQ(block.match_count_word(0, bit(17)), 1);
        EXPECT_EQ(block.match_count_word(0, bit(16)), 0);
        EXPECT_EQ(block.match_count_word(1, bit(17)), 0);

        EXPECT_EQ(block.match_mask_word(0, bit(17)), (1ULL << 17));
        EXPECT_EQ(block.match_mask_word(0, bit(16)), 0);
        EXPECT_EQ(block.match_count_word(1, bit(17)), 0);

        EXPECT_EQ(block.match_mask_word(0, bits_between(0,64)), (1ULL << 17));

    }

    TEST(StorageBitArrayTest, SimpleCheckMaskAPI64) {
        BitArray bits(16);

        constexpr std::uint64_t byte_offset = 69;
        constexpr std::uint64_t lower = bit(33);
        constexpr std::uint64_t upper = bit(43);

        constexpr auto mask_lt = bits_between(0, 33);
        constexpr auto mask_ib = bits_between(34, 43);
        constexpr auto mask_gt = bits_between(44, 64);

        auto& block = bits.block(byte_offset);
        constexpr auto word_index = (byte_offset % 64) / 8;

        block.or_word(word_index, lower);
        block.or_word(word_index, upper);

        EXPECT_FALSE(block.any_word(word_index, mask_gt));
        EXPECT_FALSE(block.any_word(word_index, mask_ib));
        EXPECT_FALSE(block.any_word(word_index, mask_lt));
        EXPECT_FALSE(block.any_word(word_index, mask_gt | mask_ib | mask_lt));
        EXPECT_FALSE(block.any_word(word_index, bit(37)));

        EXPECT_TRUE(block.any_word(word_index, mask_gt | upper));
        EXPECT_TRUE(block.any_word(word_index, upper | mask_ib | lower));
        EXPECT_TRUE(block.any_word(word_index, lower | mask_lt));
        EXPECT_TRUE(block.any_word(word_index, upper));
        EXPECT_TRUE(block.any_word(word_index, lower));

        EXPECT_FALSE(block.any_word(word_index + 1, lower | upper));

        EXPECT_EQ(block.match_count_word(word_index, bits_between(0,64)), 2);
        EXPECT_EQ(block.match_count_word(word_index + 1, bits_between(0,64)), 0);

        EXPECT_EQ(block.match_mask_word(word_index, bits_between(0,64)), lower | upper);
        EXPECT_EQ(block.match_mask_word(word_index + 1, bits_between(0,64)), 0);
    }

    TEST(StorageBitArrayTest, SimpleCheckMaskAPI16) {
        BitArray bits(16);

        constexpr std::uint64_t byte_offset = 69;
        constexpr std::uint64_t lower = bit(7);
        constexpr std::uint64_t upper = bit(12);

        constexpr auto word_index = (byte_offset % 64) / 8;
        constexpr auto byte_in_word = (byte_offset % 64) % 8;
        constexpr auto shift = byte_in_word * 8;

        constexpr auto mask_lt = bits_between(0, 7) << shift;
        constexpr auto mask_ib = bits_between(8, 12) << shift;
        constexpr auto mask_gt = bits_between(13, 16) << shift;

        auto& block = bits.block(byte_offset);

        block.or_word(word_index, lower << shift);
        block.or_word(word_index, upper << shift);

        EXPECT_FALSE(block.any_word(word_index, mask_gt));
        EXPECT_FALSE(block.any_word(word_index, mask_ib));
        EXPECT_FALSE(block.any_word(word_index, mask_lt));
        EXPECT_FALSE(block.any_word(word_index, mask_gt | mask_ib | mask_lt));
        EXPECT_FALSE(block.any_word(word_index, bit(10) << shift));

        EXPECT_TRUE(block.any_word(word_index, mask_gt | (upper << shift)));
        EXPECT_TRUE(block.any_word(word_index, (upper << shift) | mask_ib | (lower << shift)));
        EXPECT_TRUE(block.any_word(word_index, (lower << shift) | mask_lt));
        EXPECT_TRUE(block.any_word(word_index, upper << shift));
        EXPECT_TRUE(block.any_word(word_index, lower << shift));

        EXPECT_FALSE(block.any_word(word_index + 1, (lower | upper) << shift));

        EXPECT_EQ(block.match_count_word(word_index, bits_between(0,16) << shift), 2);
        EXPECT_EQ(block.match_count_word(word_index + 1, bits_between(0,16) << shift), 0);
        EXPECT_EQ(block.match_count_word(word_index, bits_between(0,16) << (shift - 16)), 0);

        EXPECT_EQ(block.match_mask_word(word_index, bits_between(0,16) << shift), (lower | upper) << shift);
        EXPECT_EQ(block.match_mask_word(word_index + 1, bits_between(0,16) << shift), 0);
        EXPECT_EQ(block.match_mask_word(word_index, bits_between(0,16) << (shift - 16)), 0);
    }

    TEST(StorageBitArrayTest, BlockMaskAcrossAllWords) {
        BitArray bits(1);

        std::array<std::uint64_t, 8> mask{};
        mask[3] = bit(11);

        auto& block = bits.block(0);
        block.or_block(mask);

        EXPECT_TRUE(block.any_block(mask));
        EXPECT_FALSE(block.any_block(std::array<std::uint64_t, 8>{}));

        EXPECT_EQ(block.match_count_block(std::array<std::uint64_t, 8>{}), 0);
        EXPECT_EQ(block.match_count_block(mask), 1);
        EXPECT_EQ(block.match_mask_block(mask), mask);
    }

}
