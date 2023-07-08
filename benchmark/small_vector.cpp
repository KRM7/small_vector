#include <benchmark/benchmark.h>
#include <small_vector.hpp>
#include <vector>
#include <string>

inline constexpr size_t SMALL_SIZE = 4;
inline constexpr size_t LARGE_SIZE = 100;
inline constexpr const char* DUMMY_STRING = "long data string for the non trivial type used in the benchmarks";

using TrivialType = int;
struct NonTrivialType
{
    NonTrivialType() : data(DUMMY_STRING) {}
    NonTrivialType(int) : data(DUMMY_STRING) {}

    NonTrivialType(const NonTrivialType& other) : data(other.data) {}
    NonTrivialType(NonTrivialType&& other) : data(std::move(other.data)) {}

    NonTrivialType& operator=(const NonTrivialType& rhs) { data = rhs.data; return *this; }
    NonTrivialType& operator=(NonTrivialType&& rhs) { data = std::move(rhs.data); return *this; }

    ~NonTrivialType() noexcept {}

    friend bool operator==(const NonTrivialType&, const NonTrivialType&) = default;

    std::string data;
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
void benchmark_push_back(benchmark::State& state)
{
    size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec;
        for (size_t i = 0; i < size; i++)
        {
            vec.push_back(1);
            benchmark::DoNotOptimize(vec);
        }
    }
}

BENCHMARK(benchmark_push_back<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_push_back<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_push_back<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_push_back<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_reserve_push_back(benchmark::State& state)
{
    size_t size = state.range(0);

    for (auto _ : state)
    {
        V vec;
        vec.reserve(size);
        for (size_t i = 0; i < size; i++)
        {
            vec.push_back(1);
            benchmark::DoNotOptimize(vec);
        }
    }
}

BENCHMARK(benchmark_reserve_push_back<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_reserve_push_back<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_reserve_push_back<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_reserve_push_back<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

/* ----------------------------------------------------------------------------------------------------------- */

template<typename V>
void benchmark_index_operator(benchmark::State& state)
{
    const size_t size = state.range(0);
    V vec(size, 0);

    for (auto _ : state) { benchmark::DoNotOptimize(vec[2]); }
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
        state.PauseTiming();
        V vec(size, 1);
        vec.reserve(size + 1);
        state.ResumeTiming();

        vec.insert(vec.begin(), 1);
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
        state.PauseTiming();
        V vec(size, 1);
        state.ResumeTiming();

        vec.erase(vec.begin());
        benchmark::DoNotOptimize(vec);
    }
}

BENCHMARK(benchmark_erase<std::vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_erase<small_vector<TrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);

BENCHMARK(benchmark_erase<std::vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
BENCHMARK(benchmark_erase<small_vector<NonTrivialType>>)->ArgName("size")->Arg(SMALL_SIZE)->Arg(LARGE_SIZE);
