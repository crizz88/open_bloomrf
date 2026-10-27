//
// Created by crizz on 20.10.26.
//

#include <cstdio>
/**
* Copyright (c) 2026 cR!zZ.
 * All rights reserved.
 *
 * This project is not licensed for redistribution or commercial use.
 *
 */

#include "open_bloomrf/bloomrf.h"

int main(int argc, char **argv) {

    bloomrf_t* filter = bloomrf_create(1000, 0.01);

    // bloomrf_insert(filter, 42);
    // bloomrf_insert(filter, 1337);
    //
    // std::printf("contains 42: %d\n", bloomrf_contains(filter, 42));
    // std::printf("contains 99: %d\n", bloomrf_contains(filter, 99));

    bloomrf_destroy(filter);
}
