/**
* Copyright (c) 2026 cR!zZ.
 * All rights reserved.
 *
 * This project is not licensed for redistribution or commercial use.
 *
 */
#pragma once

#include <cstddef>
#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bloomrf bloomrf_t;

bloomrf_t* bloomrf_create(size_t expected_items, double false_positive_rate);
void bloomrf_destroy(bloomrf_t* filter);

int bloomrf_insert(bloomrf_t* filter, uint64_t key);
int bloomrf_contains(bloomrf_t* filter, uint64_t key);

#ifdef __cplusplus
}
#endif
