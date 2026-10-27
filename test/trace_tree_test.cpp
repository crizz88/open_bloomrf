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

#include <string>
#include <gtest/gtest.h>

#include <open_bloomrf/dyadic/trace_level.hpp>

#include "open_bloomrf/storage/storage_layout.hpp"

namespace {

    using bloomrf::TraceLevel;
    using bloomrf::StorageAreaType;

    TEST(TraceTreeTest, DerivesFieldsFromWidth512) {
        const TraceLevel tree{10, 1};

        EXPECT_EQ(tree.width(), 512);
        EXPECT_EQ(tree.levels_in_tree(), 9);
        EXPECT_EQ(tree.levels_handled(), 10);
        EXPECT_EQ(tree.location_mapper(), 6);
        EXPECT_EQ(tree.subkey_helper(), 511);
    }

    TEST(TraceTreeTest, DerivesFieldsFromWidth256) {
        const TraceLevel tree{9, 1};

        EXPECT_EQ(tree.width(), 256);
        EXPECT_EQ(tree.levels_in_tree(), 8);
        EXPECT_EQ(tree.levels_handled(), 9);
        EXPECT_EQ(tree.location_mapper(), 5);
        EXPECT_EQ(tree.subkey_helper(), 255);
    }

    TEST(TraceTreeTest, DerivesFieldsFromWidth128) {
        const TraceLevel tree{8, 1};

        EXPECT_EQ(tree.width(), 128);
        EXPECT_EQ(tree.levels_in_tree(), 7);
        EXPECT_EQ(tree.levels_handled(), 8);
        EXPECT_EQ(tree.location_mapper(), 4);
        EXPECT_EQ(tree.subkey_helper(), 127);
    }

    TEST(TraceTreeTest, DerivesFieldsFromWidth64) {
        const TraceLevel tree{7, 1};

        EXPECT_EQ(tree.width(), 64);
        EXPECT_EQ(tree.levels_in_tree(), 6);
        EXPECT_EQ(tree.levels_handled(), 7);
        EXPECT_EQ(tree.location_mapper(), 3);
        EXPECT_EQ(tree.subkey_helper(), 63);
    }

    TEST(TraceTreeTest, DerivesFieldsFromWidth32) {
        const TraceLevel tree{6, 1};

        EXPECT_EQ(tree.width(), 32);
        EXPECT_EQ(tree.levels_in_tree(), 5);
        EXPECT_EQ(tree.levels_handled(), 6);
        EXPECT_EQ(tree.location_mapper(), 2);
        EXPECT_EQ(tree.subkey_helper(), 31);
    }

    TEST(TraceTreeTest, DerivesFieldsFromWidth16) {
        const TraceLevel tree{5, 1};

        EXPECT_EQ(tree.width(), 16);
        EXPECT_EQ(tree.levels_in_tree(), 4);
        EXPECT_EQ(tree.levels_handled(), 5);
        EXPECT_EQ(tree.location_mapper(), 1);
        EXPECT_EQ(tree.subkey_helper(), 15);
    }

    TEST(TraceTreeTest, DerivesFieldsFromWidth8) {
        const TraceLevel tree{4, 1};

        EXPECT_EQ(tree.width(), 8);
        EXPECT_EQ(tree.levels_in_tree(), 3);
        EXPECT_EQ(tree.levels_handled(), 4);
        EXPECT_EQ(tree.location_mapper(), 0);
        EXPECT_EQ(tree.subkey_helper(), 7);
    }

    TEST(TraceTreeTest, DerivesFieldsFromWidth4) {
        const TraceLevel tree{3, 1};

        EXPECT_EQ(tree.width(), 4);
        EXPECT_EQ(tree.levels_in_tree(), 2);
        EXPECT_EQ(tree.levels_handled(), 3);
        EXPECT_EQ(tree.location_mapper(), 0);
        EXPECT_EQ(tree.subkey_helper(), 3);
    }

    TEST(TraceTreeTest, DerivesFieldsFromWidth2) {
        const TraceLevel tree{2, 1};

        EXPECT_EQ(tree.width(), 2);
        EXPECT_EQ(tree.levels_in_tree(), 1);
        EXPECT_EQ(tree.levels_handled(), 2);
        EXPECT_EQ(tree.location_mapper(), 0);
        EXPECT_EQ(tree.subkey_helper(), 1);
    }

    TEST(TraceTreeTest, DerivesFieldsFromWidthInvalid) {
        EXPECT_THROW(TraceLevel(11, 1), std::invalid_argument);
        EXPECT_THROW(TraceLevel(0, 1), std::invalid_argument);
        EXPECT_THROW(TraceLevel(1, 1), std::invalid_argument);
        EXPECT_THROW(TraceLevel(1, 1, StorageAreaType::Exact), std::invalid_argument);
        EXPECT_THROW(TraceLevel(1, 1, StorageAreaType::Mid), std::invalid_argument);
        EXPECT_THROW(TraceLevel(1, 1, StorageAreaType::Tail), std::invalid_argument);
        EXPECT_THROW(TraceLevel(11, 1, StorageAreaType::Mid), std::invalid_argument);
        EXPECT_THROW(TraceLevel(11, 1, StorageAreaType::Tail), std::invalid_argument);

        EXPECT_NO_THROW(TraceLevel(11, 1, StorageAreaType::Exact));
        EXPECT_NO_THROW(TraceLevel(35, 1, StorageAreaType::Exact));

    }

    TEST(TraceTreeTest, ExactStorage) {
        const TraceLevel tree{7, 1, StorageAreaType::Exact};

        EXPECT_TRUE(tree.is_exact());
    }

    TEST(TraceTreeTest, ExactStorageIsDisabledByDefault) {
        const TraceLevel tree{7, 1};

        EXPECT_FALSE(tree.is_exact());
    }

    TEST(TraceTreeTest, ExactStorageIsDisabledByDefault2) {
        const TraceLevel tree{7, 1, bloomrf::StorageAreaType::None};

        EXPECT_FALSE(tree.is_exact());
    }

    TEST(TraceTreeTest, ExactStorageIsDisabledByDefault3) {
        const TraceLevel tree{7, 1, bloomrf::StorageAreaType::Mid};

        EXPECT_FALSE(tree.is_exact());
    }

    TEST(TraceTreeTest, ExactStorageIsDisabledByDefault4) {
        const TraceLevel tree{7, 1, bloomrf::StorageAreaType::Tail};

        EXPECT_FALSE(tree.is_exact());
    }

    TEST(TraceTreeTest, RejectsInvalidWidth) {
        EXPECT_THROW(
            TraceLevel(0, 1),
            std::invalid_argument
        );

        EXPECT_THROW(
            TraceLevel(65, 0),
            std::invalid_argument
        );
    }

    TEST(TraceTreeTest, RejectsInvalidArea) {
        EXPECT_THROW(
            TraceLevel(7, 1, static_cast<bloomrf::StorageAreaType>(4)),
            std::invalid_argument
        );
    }

    TEST(TraceTreeTest, ToStringContainsCoreValues) {
        const TraceLevel tree{7, 1};
        const auto text = tree.to_string();

        std::cout << text << std::endl;

        EXPECT_NE(text.find("width:            64"), std::string::npos);
        EXPECT_NE(text.find("area"), std::string::npos);
    }

} // namespace