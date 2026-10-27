/**
* Copyright (c) 2026 cR!zZ.
 * All rights reserved.
 *
 * This project is not licensed for redistribution or commercial use.
 *
 */

#include "open_bloomrf/bloomrf.h"

#include <iostream>


bloomrf_t* bloomrf_create(size_t expected_items, double false_positive_rate) {
    (void)false_positive_rate;
    (void)expected_items;
    return nullptr;
}

void bloomrf_destroy(bloomrf_t* filter) {
    (void)filter;
}


int bloomrf_insert(bloomrf_t* filter, uint64_t key) {
    (void)filter;
    (void)key;
    return 0;
}

int bloomrf_contains(bloomrf_t* filter, uint64_t key) {
    (void)filter;
    return key == 42;
}