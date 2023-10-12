#include <benchmark/benchmark.h>
#include <small_vector.hpp>
#include <vector>
#include <string>

inline constexpr size_t SMALL_SIZE = 4;
inline constexpr size_t LARGE_SIZE = 100;

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_ctor_size(benchmark::State& state)
{
    const size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec(size);
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_ctor_size<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_ctor_size<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_ctor_size_value(benchmark::State& state)
{
    const size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec(size, 2);
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_ctor_size_value<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_ctor_size_value<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_ctor_range(benchmark::State& state)
{
    const size_t size = state.range(0);
    const V src(size, 0);

    for (auto _ : state)
    {
        V vec(src.begin(), src.end());
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_ctor_range<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_ctor_range<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_push_back(benchmark::State& state)
{
    size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec;
        for (size_t i = 0; i < size; i++) { vec.push_back(int(i)); }
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_push_back<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_push_back<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_reserve_push_back(benchmark::State& state)
{
    size_t size = state.range(0);
    V vec(size);

    for (auto _ : state)
    {
        vec.resize(0);
        for (size_t i = 0; i < size; i++) { vec.push_back(int(i)); }
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_reserve_push_back<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_reserve_push_back<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_index_operator(benchmark::State& state)
{
    const size_t size = state.range(0);
    V vec(size, 0);

    for (auto _ : state) { benchmark::DoNotOptimize(vec[2]); }
}

BENCHMARK(benchmark_index_operator<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_index_operator<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_iteration(benchmark::State& state)
{
    const size_t size = state.range(0);
    V vec(size, 0);

    for (auto _ : state)
    {
        for (auto& elem : vec) { benchmark::DoNotOptimize(elem); }
    }
}

BENCHMARK(benchmark_iteration<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_iteration<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_assign(benchmark::State& state)
{
    const size_t size = state.range(0);
    const V src(size, 1);
    V dest(size, 2);

    for (auto _ : state)
    {
        dest.resize(0);
        benchmark::DoNotOptimize(dest);

        dest.assign(src.begin(), src.end());
        benchmark::DoNotOptimize(dest);
    }
}

BENCHMARK(benchmark_assign<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_assign<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_swap(benchmark::State& state)
{
    const size_t size = state.range(0);

    V left(size, 1);
    V right(size, 2);

    for (auto _ : state)
    {
        using std::swap;
        swap(left, right);
        benchmark::DoNotOptimize(left);
        benchmark::DoNotOptimize(right);
    }
}

BENCHMARK(benchmark_swap<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_swap<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_insert(benchmark::State& state)
{
    const size_t size = state.range(0);
    V vec(size, 1);
    vec.reserve(size + 1);

    for (auto _ : state)
    {     
        vec.insert(vec.begin(), 1);
        benchmark::DoNotOptimize(vec);

        vec.resize(size);
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_insert<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_insert<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_erase(benchmark::State& state)
{
    const size_t size = state.range(0);
    V vec(size, 1);

    for (auto _ : state)
    {
        vec.erase(vec.begin());
        benchmark::DoNotOptimize(vec);

        vec.resize(size);
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_erase<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_erase<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
