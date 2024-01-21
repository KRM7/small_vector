#include <benchmark/benchmark.h>
#include <small_vector.hpp>
#include <vector>
#include <string>

inline constexpr size_t SMALL_SIZE = 4;
inline constexpr size_t LARGE_SIZE = 100;

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_construct_from_size(benchmark::State& state)
{
    const size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec(size);
        benchmark::DoNotOptimize(vec);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_construct_from_size<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_construct_from_size<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_construct_from_size_value(benchmark::State& state)
{
    const size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec(size, 2);
        benchmark::DoNotOptimize(vec);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_construct_from_size_value<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_construct_from_size_value<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_construct_from_range(benchmark::State& state)
{
    const V src(state.range(0), 0);

    for (auto _ : state)
    {
        V vec(src.begin(), src.end());
        benchmark::DoNotOptimize(vec);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_construct_from_range<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_construct_from_range<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_assign_reserved(benchmark::State& state)
{
    const size_t size = state.range(0);

    V src(size, 1);
    V dst(size, 2);

    for (auto _ : state)
    {
        dst.resize(0);
        benchmark::DoNotOptimize(dst);

        dst.assign(src.begin(), src.end());
        benchmark::DoNotOptimize(dst);

        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_assign_reserved<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_assign_reserved<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

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
        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_swap<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_swap<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_resize(benchmark::State& state)
{
    const size_t size = state.range(0);

    V vec(size);

    for (auto _ : state)
    {
        vec.resize(size - 1);
        benchmark::DoNotOptimize(vec);

        vec.resize(size);
        benchmark::DoNotOptimize(vec);

        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_resize<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_resize<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_push_back_reserved(benchmark::State& state)
{
    const size_t size = state.range(0);

    V vec(size + 1);

    for (auto _ : state)
    {
        vec.resize(size);
        benchmark::DoNotOptimize(vec);

        vec.push_back(3);
        benchmark::DoNotOptimize(vec);

        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_push_back_reserved<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_push_back_reserved<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_push_back_reallocate(benchmark::State& state)
{
    const size_t final_size = state.range(0);
    const size_t start_size = small_vector<int>::small_capacity();

    for (auto _ : state)
    {
        V vec(start_size);

        for (size_t i = 0; i < final_size; i++)
        {
            benchmark::DoNotOptimize(vec);
            vec.push_back(static_cast<int>(i));
            benchmark::ClobberMemory();
        }
    }
}

BENCHMARK(benchmark_push_back_reallocate<std::vector<int>>)->ArgName("size")->Arg(LARGE_SIZE);
BENCHMARK(benchmark_push_back_reallocate<small_vector<int>>)->ArgName("size")->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_insert_one_reserved(benchmark::State& state)
{
    const size_t size = state.range(0);

    V vec(size + 1, 1);

    for (auto _ : state)
    {
        vec.resize(size);
        benchmark::DoNotOptimize(vec);

        vec.insert(vec.begin(), 3);
        benchmark::DoNotOptimize(vec);

        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_insert_one_reserved<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_insert_one_reserved<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_insert_one_reallocate(benchmark::State& state)
{
    const size_t final_size = state.range(0);
    const size_t start_size = small_vector<int>::small_capacity();

    for (auto _ : state)
    {
        V vec(start_size);

        for (size_t i = 0; i < final_size; i++)
        {
            benchmark::DoNotOptimize(vec);
            vec.insert(vec.begin(), static_cast<int>(i));
            benchmark::ClobberMemory();
        }
    }
}

BENCHMARK(benchmark_insert_one_reallocate<std::vector<int>>)->ArgName("size")->Arg(LARGE_SIZE);
BENCHMARK(benchmark_insert_one_reallocate<small_vector<int>>)->ArgName("size")->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_insert_range_reserved(benchmark::State& state)
{
    const size_t size = state.range(0);

    V vec(size + 3, 1);

    for (auto _ : state)
    {
        vec.resize(size);
        benchmark::DoNotOptimize(vec);

        vec.insert(vec.begin(), { 1, 2, 3 });
        benchmark::DoNotOptimize(vec);

        benchmark::ClobberMemory();
    }
}

BENCHMARK(benchmark_insert_range_reserved<std::vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_insert_range_reserved<small_vector<int>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_insert_range_reallocate(benchmark::State& state)
{
    const size_t final_size = state.range(0);
    const size_t start_size = small_vector<int>::small_capacity();

    for (auto _ : state)
    {
        V vec(start_size);

        while (vec.size() < final_size)
        {
            benchmark::DoNotOptimize(vec);
            vec.insert(vec.begin(), { 1, 2, 3 });
            benchmark::ClobberMemory();
        }
    }
}

BENCHMARK(benchmark_insert_range_reallocate<std::vector<int>>)->ArgName("size")->Arg(LARGE_SIZE);
BENCHMARK(benchmark_insert_range_reallocate<small_vector<int>>)->ArgName("size")->Arg(LARGE_SIZE);

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
