# open_bloomrf

**open_bloomrf** is a work-in-progress C++ library rewrite for fast point &
range queries over hierarchical, dyadic data structures based on 
[**bloomRF: On Performing Range-Queries in Bloom-Filters with 
Piecewise-Monotone Hash Functions and Prefix Hashing**](
https://openproceedings.org/2023/conf/edbt/paper-190.pdf)
(EDBT Best Paper Award 2023).

The project explores a storage design that combines:

- Point and range probe support in bloom filters
- An implicit dyadic trace tree for hierarchical range pruning
- Trace-level-aware accessors
- Cache-line-sized bitmap blocks for compact hierarchical membership state
- SIMD-friendly block operations
- Online-support by supporting atomic insertions

> **Status:** Early development.  
> The core storage abstractions, the implicit dyadic scheme, and initial 
> tests are in place. The query and accessor layers are under active 
> development.

## Motivation

The goal is to support efficient range queries where a query does not need to
inspect every element. Instead, the system progressively narrows a set of
candidate ranges while descending through a dyadic tree.

At each level, only traces that may still contain a matching value remain in
the candidate set. Storage is organized so that these checks can be performed
using compact bit operations rather than per-element comparisons.

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Roadmap

- [x] Cache-line bitmap blocks
- [x] Word- and block-level OR and mask operations
- [x] Implicit dyadic trace scheme
- [x] Storage and trace-tree tests
- [ ] Trace accessor layer
- [ ] Range-query pruning
- [ ] Advanced Dyadic Trace Tree Configurations
- [ ] Benchmarks
- [ ] Serialization support

## License

Copyright (c) 2026 cR!zZ.  
All rights reserved.

This project is not licensed for redistribution or commercial use.