```shell
clang-cl version 15.0.1 -std:c++20 -O2 -flto -DNDEBUG
```

```shell
small_vector_benchmark --benchmark_enable_random_interleaving=true --benchmark_repetitions=5
```

```text
---------------------------------------------------------------------------------------------------
Benchmark                                                         Time             CPU   Iterations
---------------------------------------------------------------------------------------------------
// ctor_size
benchmark_ctor_size<std::vector<int>>/size:4_median             106 ns          100 ns            5
benchmark_ctor_size<small_vector<int>>/size:4_median           6.88 ns         6.49 ns            5

benchmark_ctor_size<std::vector<int>>/size:100_median           148 ns          109 ns            5
benchmark_ctor_size<small_vector<int>>/size:100_median          104 ns          100 ns            5

// ctor_size_value
benchmark_ctor_size_value<std::vector<int>>/size:4_median      95.1 ns         96.3 ns            5
benchmark_ctor_size_value<small_vector<int>>/size:4_median     5.46 ns         5.47 ns            5

benchmark_ctor_size_value<std::vector<int>>/size:100_median     123 ns          117 ns            5
benchmark_ctor_size_value<small_vector<int>>/size:100_median   97.5 ns         95.9 ns            5

// ctor_range
benchmark_ctor_range<std::vector<int>>/size:4_median            100 ns         94.2 ns            5
benchmark_ctor_range<small_vector<int>>/size:4_median          5.95 ns         5.72 ns            5

benchmark_ctor_range<std::vector<int>>/size:100_median          112 ns          105 ns            5
benchmark_ctor_range<small_vector<int>>/size:100_median         109 ns          109 ns            5

// push_back
benchmark_push_back<std::vector<int>>/size:4_median             474 ns          484 ns            5
benchmark_push_back<small_vector<int>>/size:4_median           8.98 ns         9.00 ns            5

benchmark_push_back<std::vector<int>>/size:100_median          1846 ns         1800 ns            5
benchmark_push_back<small_vector<int>>/size:100_median          938 ns          921 ns            5

// reserve_push_back
benchmark_reserve_push_back<std::vector<int>>/size:4_median    7.20 ns         7.11 ns            5
benchmark_reserve_push_back<small_vector<int>>/size:4_median   7.83 ns         7.85 ns            5

benchmark_reserve_push_back<std::vector<int>>/size:100_median   275 ns          273 ns            5
benchmark_reserve_push_back<small_vector<int>>/size:100_median  274 ns          273 ns            5

// index
benchmark_index_operator<std::vector<int>>/size:4_median      0.463 ns        0.453 ns            5
benchmark_index_operator<small_vector<int>>/size:4_median     0.461 ns        0.469 ns            5

benchmark_index_operator<std::vector<int>>/size:100_median    0.476 ns        0.469 ns            5
benchmark_index_operator<small_vector<int>>/size:100_median   0.482 ns        0.469 ns            5

// iteration
benchmark_iteration<std::vector<int>>/size:4_median            4.38 ns         4.24 ns            5
benchmark_iteration<small_vector<int>>/size:4_median           4.38 ns         4.33 ns            5

benchmark_iteration<std::vector<int>>/size:100_median          57.4 ns         57.2 ns            5
benchmark_iteration<small_vector<int>>/size:100_median         58.5 ns         58.6 ns            5

// assign
benchmark_assign<std::vector<int>>/size:4_median               12.9 ns         11.2 ns            5
benchmark_assign<small_vector<int>>/size:4_median              14.9 ns         11.5 ns            5

benchmark_assign<std::vector<int>>/size:100_median             21.7 ns         17.8 ns            5
benchmark_assign<small_vector<int>>/size:100_median            23.7 ns         19.3 ns            5

// swap
benchmark_swap<std::vector<int>>/size:4_median                 2.71 ns         2.73 ns            5
benchmark_swap<small_vector<int>>/size:4_median                12.2 ns         11.9 ns            5

benchmark_swap<std::vector<int>>/size:100_median               2.74 ns         2.68 ns            5
benchmark_swap<small_vector<int>>/size:100_median              2.67 ns         2.57 ns            5

// insert
benchmark_insert<std::vector<int>>/size:4_median               12.1 ns         9.49 ns            5
benchmark_insert<small_vector<int>>/size:4_median              11.1 ns         9.07 ns            5

benchmark_insert<std::vector<int>>/size:100_median             29.4 ns         21.4 ns            5
benchmark_insert<small_vector<int>>/size:100_median            29.9 ns         22.5 ns            5

// erase
benchmark_erase<std::vector<int>>/size:4_median                11.7 ns         11.2 ns            5
benchmark_erase<small_vector<int>>/size:4_median               13.0 ns         12.9 ns            5

benchmark_erase<std::vector<int>>/size:100_median              18.8 ns         18.6 ns            5
benchmark_erase<small_vector<int>>/size:100_median             22.9 ns         22.5 ns            5

```