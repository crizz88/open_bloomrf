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
struct LevelFpr {
    uint32_t level;
    double fpr;
    double true_positive;
    double true_negative;
    double false_positive;
};

FprEstimate estimate_fpr(
    const BloomRfConfig& config,
    uint64_t inserted_items,
    const WorkloadProfile& workload
);