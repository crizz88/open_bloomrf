//
// Created by crizz on 23.10.26.
//
/**
* Copyright (c) 2026 cR!zZ.
 * All rights reserved.
 *
 * This project is not licensed for redistribution or commercial use.
 *
 */

#include <gtest/gtest.h>

#include <open_bloomrf/dyadic/dyadic_trace_tree.hpp>

namespace {

using bloomrf::DyadicTraceTree;
using bloomrf::StorageAreaType;
using bloomrf::StorageLayout;
using bloomrf::StorageRegion;
using bloomrf::TraceLevel;

    TEST(DyadicTraceTreeTest, StoresTopLevelAndLevelsSimple) {
        std::vector<TraceLevel> levels;
        for (int i = 0; i < 9; ++i) {
            levels.emplace_back(7, 1);
        }

        const DyadicTraceTree tree{
            levels,
            StorageLayout::flat(1024)
        };

        EXPECT_EQ(tree.top_level(), 63);
        EXPECT_EQ(tree.levels().size(), levels.size());
        EXPECT_EQ(tree.level(0).width(), 64);
        EXPECT_EQ(tree.level(1).area(), StorageAreaType::None);
    }

TEST(DyadicTraceTreeTest, StoresTopLevelAndLevels) {
    const std::vector<TraceLevel> levels{
        {21, 1, StorageAreaType::Exact},
        {2, 3, StorageAreaType::Mid},
        {2, 2, StorageAreaType::Mid},
        {4, 2, StorageAreaType::Mid},
        {7, 1, StorageAreaType::Mid},
        {7, 1, StorageAreaType::Mid},
        {7, 1, StorageAreaType::Tail},
        {7, 1, StorageAreaType::Tail},
        {7, 1, StorageAreaType::Tail}
    };

    const DyadicTraceTree tree{
        levels,
        StorageLayout::segmented(1024, 512, 448)
    };

    EXPECT_EQ(tree.top_level(), 64);
    EXPECT_EQ(tree.levels().size(), levels.size());
    EXPECT_EQ(tree.level(5).width(), 64);
    EXPECT_EQ(tree.level(0).area(), StorageAreaType::Exact);
    EXPECT_EQ(tree.level(1).area(), StorageAreaType::Mid);
    EXPECT_EQ(tree.level(2).area(), StorageAreaType::Mid);
    EXPECT_EQ(tree.level(3).area(), StorageAreaType::Mid);
    EXPECT_EQ(tree.level(8).area(), StorageAreaType::Tail);
    const auto exact = tree.region_for(0);
    const auto mid = tree.region_for(1);
    const auto tail = tree.region_for(8);

    EXPECT_EQ(exact.byte_offset(), 0);
    EXPECT_EQ(exact.byte_length(), 512);
    EXPECT_EQ(exact.end_byte(), 512);

    EXPECT_EQ(mid.byte_offset(), 512);
    EXPECT_EQ(mid.byte_length(), 448);
    EXPECT_EQ(mid.end_byte(), 960);

    EXPECT_EQ(tail.byte_offset(), 960);
    EXPECT_EQ(tail.byte_length(), 64);
    EXPECT_EQ(tail.end_byte(), 1024);
}

TEST(DyadicTraceTreeTest, RejectsEmptyLevelList) {
    const std::vector<TraceLevel> levels{};

    EXPECT_THROW(
        DyadicTraceTree(levels, StorageLayout::flat(1024)),
        std::invalid_argument
    );
}

TEST(DyadicTraceTreeTest, RejectsInvalidLevelIndex) {
    const std::vector<TraceLevel> levels{
        TraceLevel{8, 1, StorageAreaType::None}
    };

    const DyadicTraceTree tree{
        levels,
        StorageLayout::flat(4096)
    };

    EXPECT_THROW(auto trace_level_ret = tree.level(1), std::out_of_range);
    EXPECT_THROW(auto region_ret = tree.region_for(1), std::out_of_range);
}

TEST(DyadicTraceTreeTest, RegionsAreContiguous) {
    const std::vector<TraceLevel> levels{
        TraceLevel{8, 1, StorageAreaType::Exact},
        TraceLevel{7, 1, StorageAreaType::Mid},
        TraceLevel{7, 1, StorageAreaType::Tail}
    };

    const DyadicTraceTree tree{
        levels,
        StorageLayout::segmented(1024, 512, 448)
    };

    const auto exact = tree.region_for(0);
    const auto mid = tree.region_for(1);
    const auto tail = tree.region_for(2);

    EXPECT_EQ(exact.end_byte(), mid.byte_offset());
    EXPECT_EQ(mid.end_byte(), tail.byte_offset());
    EXPECT_EQ(tail.end_byte(), 1024);
}

} // namespace