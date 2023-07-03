#include <benchmark/benchmark.h>
#include <small_vector.hpp>
#include <vector>

inline constexpr size_t SMALL_SIZE = 4;
inline constexpr size_t LARGE_SIZE = 100;

using TrivialType = int;
struct NonTrivialType
{
    NonTrivialType() {}
    NonTrivialType(int i) : i_(2 * i + 1) {}
    NonTrivialType(const NonTrivialType&) {}
    NonTrivialType(NonTrivialType&&) {}
    NonTrivialType& operator=(const NonTrivialType&) { return *this; }
    NonTrivialType& operator=(NonTrivialType&&) { return *this; }
    ~NonTrivialType() noexcept {}
    friend bool operator==(const NonTrivialType&, const NonTrivialType&) = default;
    int i_ = 0;
};

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

BENCHMARK(benchmark_ctor_size<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_ctor_size<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_ctor_size<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_ctor_size<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_ctor_size_value(benchmark::State& state)
{
    const size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec(size, 0);
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_ctor_size_value<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_ctor_size_value<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_ctor_size_value<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_ctor_size_value<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_fill(benchmark::State& state)
{
    size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec;
        for (size_t i = 0; i < size; i++) { vec.push_back(1); }
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_fill<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_fill<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_fill<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_fill<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_reserve_fill(benchmark::State& state)
{
    size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec;
        vec.reserve(size);
        for (size_t i = 0; i < size; i++) { vec.push_back(1); }
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_reserve_fill<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_reserve_fill<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_reserve_fill<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_reserve_fill<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_index_operator(benchmark::State& state)
{
    const size_t size = state.range(0);
    V vec(size, 0);

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(vec);
        benchmark::DoNotOptimize(vec[2]);
    }
}

BENCHMARK(benchmark_index_operator<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_index_operator<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

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

BENCHMARK(benchmark_iteration<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_iteration<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_assign(benchmark::State& state)
{
    const size_t size = state.range(0);
    V src(size, 1);
    V dest(size, 2);

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(src);
        dest.assign(src.begin(), src.end());
        benchmark::DoNotOptimize(dest);
    }
}

BENCHMARK(benchmark_assign<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_assign<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_assign<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_assign<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_swap(benchmark::State& state)
{
    const size_t size = state.range(0);

    V left(size, 1);
    V right(size, 2);

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(left);
        benchmark::DoNotOptimize(right);
        using std::swap;
        swap(left, right);
        benchmark::DoNotOptimize(left);
        benchmark::DoNotOptimize(right);
    }
}

BENCHMARK(benchmark_swap<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_swap<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_swap<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_swap<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_insert(benchmark::State& state)
{
    const size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec;
        for (size_t i = 0; i < size; i++) { vec.insert(vec.begin(), 1); }
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_insert<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_insert<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_insert<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_insert<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_erase(benchmark::State& state)
{
    const size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec(size, 1);
        for (size_t i = 0; i < size; i++) { vec.erase(vec.begin()); }
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_erase<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_erase<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_erase<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_erase<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
