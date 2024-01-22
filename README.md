```shell
g++ 12.3.0 -std=c++20 -O3 -flto -DNDEBUG
```

```shell
small_vector_benchmark --benchmark_enable_random_interleaving=true --benchmark_repetitions=10
```

```text
-----------------------------------------------------------------------------------------------------------------
Benchmark                                                                       Time             CPU   Iterations
-----------------------------------------------------------------------------------------------------------------
// construct_from_size
benchmark_construct_from_size<std::vector<int>>/size:4_median                11.1 ns         11.1 ns           10
benchmark_construct_from_size<small_vector<int>>/size:4_median               1.84 ns         1.84 ns           10

benchmark_construct_from_size<std::vector<int>>/size:100_median              11.5 ns         11.5 ns           10
benchmark_construct_from_size<small_vector<int>>/size:100_median             11.5 ns         11.5 ns           10

// construct_size_value
benchmark_construct_from_size_value<std::vector<int>>/size:4_median          10.2 ns         10.2 ns           10
benchmark_construct_from_size_value<small_vector<int>>/size:4_median         1.61 ns         1.61 ns           10

benchmark_construct_from_size_value<std::vector<int>>/size:100_median        16.4 ns         16.4 ns           10
benchmark_construct_from_size_value<small_vector<int>>/size:100_median       17.5 ns         17.5 ns           10

// construct_from_range
benchmark_construct_from_range<std::vector<int>>/size:4_median               11.4 ns         11.4 ns           10
benchmark_construct_from_range<small_vector<int>>/size:4_median              2.31 ns         2.31 ns           10

benchmark_construct_from_range<std::vector<int>>/size:100_median             12.7 ns         12.7 ns           10
benchmark_construct_from_range<small_vector<int>>/size:100_median            13.4 ns         13.4 ns           10

// assign_reserved
benchmark_assign_reserved<std::vector<int>>/size:4_median                    2.53 ns         2.53 ns           10
benchmark_assign_reserved<small_vector<int>>/size:4_median                   2.32 ns         2.32 ns           10

benchmark_assign_reserved<std::vector<int>>/size:100_median                  4.38 ns         4.38 ns           10
benchmark_assign_reserved<small_vector<int>>/size:100_median                 4.61 ns         4.61 ns           10

// swap
benchmark_swap<std::vector<int>>/size:4_median                               1.34 ns         1.34 ns           10
benchmark_swap<small_vector<int>>/size:4_median                              3.92 ns         3.92 ns           10

benchmark_swap<std::vector<int>>/size:100_median                             1.34 ns         1.34 ns           10
benchmark_swap<small_vector<int>>/size:100_median                            1.84 ns         1.84 ns           10

// resize
benchmark_resize<std::vector<int>>/size:4_median                             1.61 ns         1.61 ns           10
benchmark_resize<small_vector<int>>/size:4_median                            2.77 ns         2.77 ns           10

benchmark_resize<std::vector<int>>/size:100_median                           1.61 ns         1.61 ns           10
benchmark_resize<small_vector<int>>/size:100_median                          2.77 ns         2.77 ns           10

// push_back_reserved
benchmark_push_back_reserved<std::vector<int>>/size:4_median                0.923 ns        0.923 ns           10
benchmark_push_back_reserved<small_vector<int>>/size:4_median               0.952 ns        0.952 ns           10

benchmark_push_back_reserved<std::vector<int>>/size:100_median              0.923 ns        0.923 ns           10
benchmark_push_back_reserved<small_vector<int>>/size:100_median             0.952 ns        0.952 ns           10

// push_back_reallocate
benchmark_push_back_reallocate<std::vector<int>>/size:100_median              143 ns          143 ns           10
benchmark_push_back_reallocate<small_vector<int>>/size:100_median             126 ns          126 ns           10

// insert_one_reserved
benchmark_insert_one_reserved<std::vector<int>>/size:4_median                4.63 ns         4.63 ns           10
benchmark_insert_one_reserved<small_vector<int>>/size:4_median               4.61 ns         4.61 ns           10

benchmark_insert_one_reserved<std::vector<int>>/size:100_median              8.20 ns         8.20 ns           10
benchmark_insert_one_reserved<small_vector<int>>/size:100_median             7.74 ns         7.74 ns           10

// insert_one_reallocate
benchmark_insert_one_reallocate<std::vector<int>>/size:100_median             737 ns          737 ns           10
benchmark_insert_one_reallocate<small_vector<int>>/size:100_median            747 ns          747 ns           10

// insert_range_reserved
benchmark_insert_range_reserved<std::vector<int>>/size:4_median              5.30 ns         5.30 ns           10
benchmark_insert_range_reserved<small_vector<int>>/size:4_median             6.46 ns         6.46 ns           10

benchmark_insert_range_reserved<std::vector<int>>/size:100_median            8.56 ns         8.56 ns           10
benchmark_insert_range_reserved<small_vector<int>>/size:100_median           6.45 ns         6.45 ns           10

// insert_range_reallocate
benchmark_insert_range_reallocate<std::vector<int>>/size:100_median           234 ns          233 ns           10
benchmark_insert_range_reallocate<small_vector<int>>/size:100_median          251 ns          251 ns           10

// erase_front
benchmark_erase<std::vector<int>>/size:4_median                              4.84 ns         4.84 ns           10
benchmark_erase<small_vector<int>>/size:4_median                             5.54 ns         5.54 ns           10

benchmark_erase<std::vector<int>>/size:100_median                            8.43 ns         8.43 ns           10
benchmark_erase<small_vector<int>>/size:100_median                           8.36 ns         8.36 ns           10

```