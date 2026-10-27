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
struct WorkloadProfile {
    double key_skew;             // z. B. Zipf-Alpha
    double duplicate_ratio;
    double range_correlation;    // 0 = unabhängig, 1 = stark lokal gebündelt
    std::vector<RangeHistogram> query_ranges;
};
