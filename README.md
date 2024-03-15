### `small_vector<T>`

```shell
g++ 12.3.0 -std=c++20 -O3 -flto -DNDEBUG
```

```shell
small_vector_benchmark --benchmark_min_warmup_time=2
```

```text
----------------------------------------------------------------------------------------------------------
Benchmark                                                                Time             CPU   Iterations
----------------------------------------------------------------------------------------------------------
// construct_from_size
benchmark_construct_from_size<std::vector<int>>/size:4                11.1 ns         11.1 ns     63128125
benchmark_construct_from_size<std::vector<int>>/size:100              11.5 ns         11.5 ns     60983419
benchmark_construct_from_size<small_vector<int>>/size:4               1.85 ns         1.85 ns    379357325
benchmark_construct_from_size<small_vector<int>>/size:100             11.7 ns         11.7 ns     59806415

// construct_from_size_value
benchmark_construct_from_size_value<std::vector<int>>/size:4          10.6 ns         10.6 ns     66230178
benchmark_construct_from_size_value<std::vector<int>>/size:100        17.2 ns         17.2 ns     41003124
benchmark_construct_from_size_value<small_vector<int>>/size:4         1.84 ns         1.84 ns    379785020
benchmark_construct_from_size_value<small_vector<int>>/size:100       17.7 ns         17.7 ns     39628556

// construct_from_range
benchmark_construct_from_range<std::vector<int>>/size:4               11.4 ns         11.4 ns     61351468
benchmark_construct_from_range<std::vector<int>>/size:100             12.6 ns         12.6 ns     55381324
benchmark_construct_from_range<small_vector<int>>/size:4              2.31 ns         2.31 ns    303092496
benchmark_construct_from_range<small_vector<int>>/size:100            13.2 ns         13.2 ns     53144806

// assign_reserved
benchmark_assign_reserved<std::vector<int>>/size:4                    2.30 ns         2.30 ns    300733274
benchmark_assign_reserved<std::vector<int>>/size:100                  4.93 ns         4.93 ns    141963555
benchmark_assign_reserved<small_vector<int>>/size:4                   2.30 ns         2.30 ns    303034501
benchmark_assign_reserved<small_vector<int>>/size:100                 5.28 ns         5.28 ns    132612176

// swap
benchmark_swap<std::vector<int>>/size:4                               1.34 ns         1.34 ns    518765605
benchmark_swap<std::vector<int>>/size:100                             1.33 ns         1.33 ns    522469939
benchmark_swap<small_vector<int>>/size:4                              4.15 ns         4.15 ns    168877555
benchmark_swap<small_vector<int>>/size:100                            1.84 ns         1.84 ns    380621991

// resize
benchmark_resize<std::vector<int>>/size:4                             2.54 ns         2.54 ns    275839458
benchmark_resize<std::vector<int>>/size:100                           3.67 ns         3.67 ns    197377975
benchmark_resize<small_vector<int>>/size:4                            1.87 ns         1.87 ns    376250226
benchmark_resize<small_vector<int>>/size:100                          3.28 ns         3.28 ns    215127588

// push_back_reserved
benchmark_push_back_reserved<std::vector<int>>/size:4                0.613 ns        0.613 ns   1000000000
benchmark_push_back_reserved<std::vector<int>>/size:100              0.618 ns        0.618 ns   1000000000
benchmark_push_back_reserved<small_vector<int>>/size:4               0.562 ns        0.562 ns   1000000000
benchmark_push_back_reserved<small_vector<int>>/size:100             0.617 ns        0.618 ns   1000000000

// push_back_reallocate
benchmark_push_back_reallocate<std::vector<int>>/size:100              144 ns          144 ns      4641722
benchmark_push_back_reallocate<small_vector<int>>/size:100             148 ns          148 ns      4781649

// insert_one_reserved
benchmark_insert_one_reserved<std::vector<int>>/size:4                4.66 ns         4.66 ns    150091813
benchmark_insert_one_reserved<std::vector<int>>/size:100              8.01 ns         8.01 ns     85894208
benchmark_insert_one_reserved<small_vector<int>>/size:4               4.65 ns         4.65 ns    149969588
benchmark_insert_one_reserved<small_vector<int>>/size:100             8.17 ns         8.17 ns     86572377

// insert_one_reallocate
benchmark_insert_one_reallocate<std::vector<int>>/size:100             728 ns          728 ns       965758
benchmark_insert_one_reallocate<small_vector<int>>/size:100            752 ns          752 ns       939354

// insert_range_reserved
benchmark_insert_range_reserved<std::vector<int>>/size:4              6.12 ns         6.12 ns    116646059
benchmark_insert_range_reserved<std::vector<int>>/size:100            8.62 ns         8.62 ns     82369900
benchmark_insert_range_reserved<small_vector<int>>/size:4             6.26 ns         6.26 ns    112617323
benchmark_insert_range_reserved<small_vector<int>>/size:100           6.29 ns         6.29 ns    112590153

// insert_range_reallocate
benchmark_insert_range_reallocate<std::vector<int>>/size:100           255 ns          255 ns      2707667
benchmark_insert_range_reallocate<small_vector<int>>/size:100          229 ns          229 ns      3066817

// erase_front
benchmark_erase<std::vector<int>>/size:4                              4.88 ns         4.88 ns    143511026
benchmark_erase<std::vector<int>>/size:100                            8.51 ns         8.51 ns     82108933
benchmark_erase<small_vector<int>>/size:4                             4.62 ns         4.62 ns    150378007
benchmark_erase<small_vector<int>>/size:100                           8.53 ns         8.53 ns     82985683
```