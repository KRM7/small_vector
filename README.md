```text

// ctor_size
benchmark_ctor_size<std::vector<int>>/size:4_median           112 ns         94.2 ns            5
benchmark_ctor_size<small_vector<int>>/size:4_median         6.38 ns         5.72 ns            5

benchmark_ctor_size<std::vector<int>>/size:100_median         121 ns          105 ns            5
benchmark_ctor_size<small_vector<int>>/size:100_median        126 ns          106 ns            5

// ctor_size_value
benchmark_ctor_size_value<std::vector<int>>/size:4_median          99.8 ns         97.7 ns            5
benchmark_ctor_size_value<small_vector<int>>/size:4_median         5.32 ns         5.30 ns            5

benchmark_ctor_size_value<std::vector<int>>/size:100_median         103 ns          100 ns            5
benchmark_ctor_size_value<small_vector<int>>/size:100_median        101 ns         98.4 ns            5

// ctor_range
benchmark_ctor_range<std::vector<int>>/size:4_median           123 ns          103 ns            5
benchmark_ctor_range<small_vector<int>>/size:4_median         8.05 ns         6.28 ns            5

benchmark_ctor_range<std::vector<int>>/size:100_median         169 ns          126 ns            5
benchmark_ctor_range<small_vector<int>>/size:100_median        139 ns          117 ns            5

// push_back
benchmark_push_back<std::vector<int>>/size:4_median           603 ns          519 ns            5
benchmark_push_back<small_vector<int>>/size:4_median         10.8 ns         9.38 ns            5

benchmark_push_back<std::vector<int>>/size:100_median        2527 ns         2009 ns            5
benchmark_push_back<small_vector<int>>/size:100_median       1280 ns          985 ns            5

// reserve_push_back
benchmark_reserve_push_back<std::vector<int>>/size:4_median          9.82 ns         7.50 ns            5
benchmark_reserve_push_back<small_vector<int>>/size:4_median         10.7 ns         7.85 ns            5

benchmark_reserve_push_back<std::vector<int>>/size:100_median         365 ns          285 ns            5
benchmark_reserve_push_back<small_vector<int>>/size:100_median        385 ns          279 ns            5

// index
benchmark_index_operator<std::vector<int>>/size:4_median         0.594 ns        0.484 ns            5
benchmark_index_operator<small_vector<int>>/size:4_median        0.675 ns        0.500 ns            5

benchmark_index_operator<std::vector<int>>/size:100_median       0.576 ns        0.516 ns            5
benchmark_index_operator<small_vector<int>>/size:100_median      0.624 ns        0.547 ns            5

// iteration
benchmark_iteration<std::vector<int>>/size:4_median          5.05 ns         4.29 ns            5
benchmark_iteration<small_vector<int>>/size:4_median         5.13 ns         3.99 ns            5

benchmark_iteration<std::vector<int>>/size:100_median        79.8 ns         67.0 ns            5
benchmark_iteration<small_vector<int>>/size:100_median       82.7 ns         67.0 ns            5

// assign
benchmark_assign<std::vector<int>>/size:4_median          17.2 ns         13.4 ns            5
benchmark_assign<small_vector<int>>/size:4_median         22.3 ns         17.1 ns            5

benchmark_assign<std::vector<int>>/size:100_median        25.3 ns         20.1 ns            5
benchmark_assign<small_vector<int>>/size:100_median       28.4 ns         23.6 ns            5

// swap
benchmark_swap<std::vector<int>>/size:4_median          3.31 ns         2.85 ns            5
benchmark_swap<small_vector<int>>/size:4_median         19.0 ns         14.4 ns            5

benchmark_swap<std::vector<int>>/size:100_median        3.19 ns         2.72 ns            5
benchmark_swap<small_vector<int>>/size:100_median       6.53 ns         5.15 ns            5

// insert
benchmark_insert<std::vector<int>>/size:4_median          12.2 ns         10.3 ns            5
benchmark_insert<small_vector<int>>/size:4_median         17.7 ns         14.2 ns            5

benchmark_insert<std::vector<int>>/size:100_median        33.6 ns         26.7 ns            5
benchmark_insert<small_vector<int>>/size:100_median       35.1 ns         30.7 ns            5

// erase
benchmark_erase<std::vector<int>>/size:4_median          11.7 ns         11.2 ns            5
benchmark_erase<small_vector<int>>/size:4_median         13.0 ns         12.9 ns            5

benchmark_erase<std::vector<int>>/size:100_median        18.8 ns         18.6 ns            5
benchmark_erase<small_vector<int>>/size:100_median       22.9 ns         22.5 ns            5

```