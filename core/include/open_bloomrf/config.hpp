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

struct LayerSpec {
    uint8_t height;      // Anzahl dyadischer Ebenen in dieser Schicht
    uint8_t hash_count;  // k
    uint8_t segment_id;  // Zuordnung zum Speichersegment
};

struct SegmentSpec {
    uint64_t bits;
};

struct BloomRfConfig {
    std::vector<LayerSpec> layers;
    std::vector<SegmentSpec> segments;
};

struct ConfiguratorInput {
    uint64_t expected_items;
    double bits_per_key;
    uint64_t max_query_range;
    WorkloadProfile workload;
};

BloomRfConfig plan_configuration(const ConfiguratorInput& input);
