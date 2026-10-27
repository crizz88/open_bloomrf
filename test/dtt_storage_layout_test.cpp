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
#include <ranges>
#include <string>
#include <gtest/gtest.h>

#include <open_bloomrf/storage/storage_layout.hpp>

namespace {

    using bloomrf::StorageLayout;
    using bloomrf::StorageRegion;
    using bloomrf::StorageAreaType;

    TEST(StorageRegionTest, DerivesFieldsFromOffsetLength) {
        const StorageRegion region{0, 1024};

        EXPECT_EQ(region.byte_offset(), 0);
        EXPECT_EQ(region.byte_length(), 1024);
        EXPECT_EQ(region.end_byte(), 1024);
    }

    TEST(StorageRegionTest, DerivesFieldsFromWidthVector) {
        const int vals[] = {0, 512, 960, 1024};
        const std::vector<StorageRegion> regions {{0, 512}, {512, 448}, {960, 64}};

        for (auto [i, region] : regions | std::views::enumerate) {
            EXPECT_EQ(region.byte_offset(), vals[i]);
            EXPECT_EQ(region.byte_length(), vals[i + 1] - vals[i]);
            EXPECT_EQ(region.end_byte(), vals[i + 1]);
        }
    }

    TEST(StorageRegionTest, RegionContainsOffset) {
        const int vals[] = {0, 512, 960, 1024};
        const std::vector<StorageRegion> regions {{0, 512}, {512, 448}, {960, 64}};

        for (auto [i, region] : regions | std::views::enumerate) {
            EXPECT_TRUE(region.contains(vals[i]));
            EXPECT_TRUE(region.contains((vals[i + 1] + vals[i]) / 2));
            EXPECT_TRUE(region.contains(vals[i + 1] - 1));
            EXPECT_FALSE(region.contains(vals[i + 1]));
        }
    }

    TEST(StorageLayoutTest, CreateFlatLayout) {
        const auto layout = StorageLayout::flat(1024);

        const auto region = layout.region_for(StorageAreaType::None);

        EXPECT_EQ(region.byte_offset(), 0);
        EXPECT_EQ(region.byte_length(), 1024);
        EXPECT_EQ(region.end_byte(), 1024);
    }

    TEST(StorageLayoutTest, CreateSegmentedLayout) {
        const int vals[] = {0, 512, 960, 1024};
        const auto layout = StorageLayout::segmented(1024, 512, 448);

        ASSERT_EQ(StorageLayout::segmented_areas().size(), static_cast<int>(StorageAreaType::Tail));

        for (const auto [i, area] : StorageLayout::segmented_areas() | std::views::enumerate) {
            const auto region = layout.region_for(area);
            EXPECT_EQ(region.byte_offset(), vals[i]);
            EXPECT_EQ(region.byte_length(), vals[i + 1] - vals[i]);
            EXPECT_EQ(region.end_byte(), vals[i + 1]);
        }
    }


} // namespace