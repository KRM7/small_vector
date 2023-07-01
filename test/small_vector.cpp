#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <small_vector.hpp>
#include <vector>
#include <memory>
#include <utility>
#include <cstddef>

inline constexpr size_t SMALL_SIZE = 4;
inline constexpr size_t LARGE_SIZE = 100;


using TrivialType   = int;

struct NonDefaultConstructibleType
{
    NonDefaultConstructibleType(int) {}

    friend bool operator==(const NonDefaultConstructibleType&, const NonDefaultConstructibleType&) = default;
};

struct MoveOnlyType
{
    MoveOnlyType() = default;
    MoveOnlyType(int) {}

    MoveOnlyType(const MoveOnlyType&) = delete;
    MoveOnlyType(MoveOnlyType&&) {}

    MoveOnlyType& operator=(const MoveOnlyType&) = delete;
    MoveOnlyType& operator=(MoveOnlyType&&) { return *this; }

    ~MoveOnlyType() noexcept {}

    friend bool operator==(const MoveOnlyType&, const MoveOnlyType&) = default;
};

struct ImmovableType
{
    ImmovableType() = default;
    ImmovableType(int) {}

    ImmovableType(const ImmovableType&) = delete;
    ImmovableType(ImmovableType&&)      = delete;

    ImmovableType& operator=(const ImmovableType&)  = delete;
    ImmovableType& operator=(ImmovableType&&)       = delete;

    friend bool operator==(const ImmovableType&, const ImmovableType&) = default;
};

struct NonTrivialType
{
    NonTrivialType() {}
    NonTrivialType(int) {}
    NonTrivialType(const NonTrivialType&) {}
    NonTrivialType(NonTrivialType&&) {}
    NonTrivialType& operator=(const NonTrivialType&) { return *this; }
    NonTrivialType& operator=(NonTrivialType&&) { return *this; }
    ~NonTrivialType() noexcept {}

    friend bool operator==(const NonTrivialType&, const NonTrivialType&) = default;
};


    //-----------------------------------//
    //            CONSTRUCTORS           //
    //-----------------------------------//


TEMPLATE_TEST_CASE("small_vector()", "[constructor]", TrivialType, MoveOnlyType, ImmovableType, NonTrivialType)
{
    small_vector<TestType> vec;

    REQUIRE(vec.empty());
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() > 0);
}

TEMPLATE_TEST_CASE("small_vector(Alloc)", "[constructor]", TrivialType, MoveOnlyType, ImmovableType, NonTrivialType)
{
    small_vector<TestType> vec(std::allocator<TestType>{});

    REQUIRE(vec.empty());
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.capacity() > 0);
}

TEMPLATE_TEST_CASE("small_vector(size_t)", "[constructor]", TrivialType, MoveOnlyType, ImmovableType, NonTrivialType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector<TestType> vec(size);

    REQUIRE(!vec.empty());
    REQUIRE(vec.size() == size);
    REQUIRE(vec.capacity() >= size);
}

TEMPLATE_TEST_CASE("small_vector(size_t, Alloc)", "[constructor]", TrivialType, MoveOnlyType, ImmovableType, NonTrivialType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector<TestType> vec(size, std::allocator<TestType>{});

    REQUIRE(!vec.empty());
    REQUIRE(vec.size() == size);
    REQUIRE(vec.capacity() >= size);
}

TEMPLATE_TEST_CASE("small_vector(size_t, const T&)", "[constructor]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector vec(size, TestType{ 0 });

    REQUIRE(vec.size() == size);
    REQUIRE(vec.capacity() >= size);
}

TEMPLATE_TEST_CASE("small_vector(size_t, const T&, Alloc)", "[constructor]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector vec(size, TestType{ 0 }, std::allocator<TestType>{});

    REQUIRE(vec.size() == size);
    REQUIRE(vec.capacity() >= size);
}

TEMPLATE_TEST_CASE("small_vector(Iter, Iter)", "[constructor]", TrivialType, MoveOnlyType, ImmovableType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    const std::vector<int> source(size, 2);

    small_vector<TestType> vec(source.begin(), source.end());

    REQUIRE(vec.size() == source.size());
    REQUIRE(vec.capacity() >= source.size());
}

TEMPLATE_TEST_CASE("small_vector(Iter, Iter, Alloc)", "[constructor]", TrivialType, MoveOnlyType, ImmovableType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    const std::vector<int> source(size, 2);

    small_vector<TestType> vec(source.begin(), source.end(), std::allocator<TestType>{});

    REQUIRE(vec.size() == source.size());
    REQUIRE(vec.capacity() >= source.size());
}

TEMPLATE_TEST_CASE("small_vector(initializer_list)", "[constructor]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    small_vector vec{ TestType{ 1 }, TestType{ 4 }, TestType{ 2 } };

    REQUIRE(vec.is_small());
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() >= 3);
}

TEMPLATE_TEST_CASE("small_vector(initializer_list, Alloc)", "[constructor]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    small_vector vec({ TestType{ 1 }, TestType{ 4 }, TestType{ 2 } }, std::allocator<TestType>{});

    REQUIRE(vec.is_small());
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.capacity() >= 3);
}

TEMPLATE_TEST_CASE("small_vector(const small_vector&)", "[constructor]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    const small_vector source(size, TestType{ 26 });

    small_vector vec(source);

    REQUIRE(vec.size() == source.size());
    REQUIRE(vec.capacity() >= source.size());
    REQUIRE(vec == source);
}

TEMPLATE_TEST_CASE("small_vector(const small_vector&, Alloc)", "[constructor]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    const small_vector source(size, TestType{ 26 });

    small_vector vec(source, std::allocator<TestType>{});

    REQUIRE(vec.size() == source.size());
    REQUIRE(vec.capacity() >= source.size());
    REQUIRE(vec == source);
}

TEMPLATE_TEST_CASE("small_vector(small_vector&&)", "[constructor]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector source(size, TestType{ 26 });
    small_vector source_copy(source);

    small_vector vec(std::move(source));

    REQUIRE(vec.size() == source_copy.size());
    REQUIRE(vec.capacity() >= source_copy.size());
    REQUIRE(vec == source_copy);
}

TEMPLATE_TEST_CASE("small_vector<MoveOnlyType>(small_vector&&)", "[constructor]", MoveOnlyType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector<TestType> source(size);
    small_vector<TestType> source_copy(size);

    small_vector vec(std::move(source));

    REQUIRE(vec.size() == source_copy.size());
    REQUIRE(vec.capacity() >= source_copy.size());
    REQUIRE(vec == source_copy);
}

    //-----------------------------------//
    //             ASSIGNMENT            //
    //-----------------------------------//

TEMPLATE_TEST_CASE("assign(Iter, Iter)", "[assignment]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t src_size = GENERATE(SMALL_SIZE, LARGE_SIZE + 1);
    const size_t dest_size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    const small_vector source(src_size, TestType{ 4 });
    small_vector dest(dest_size, TestType{ 3 });

    dest.assign(source.begin(), source.end());

    REQUIRE(dest.size() == source.size());
    REQUIRE(dest.capacity() >= source.size());
    REQUIRE(dest == source);
}

TEMPLATE_TEST_CASE("operator=(const small_vector&)", "[assignment]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t src_size = GENERATE(SMALL_SIZE, LARGE_SIZE + 1);
    const size_t dest_size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    const small_vector source(src_size, TestType{ 4 });
    small_vector dest(dest_size, TestType{ 3 });

    dest = source;

    REQUIRE(dest.size() == source.size());
    REQUIRE(dest.capacity() >= source.size());
    REQUIRE(dest == source);
}

TEMPLATE_TEST_CASE("operator=(small_vector&&)", "[assignment]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    WARN("Not implemented.");
}

TEMPLATE_TEST_CASE("operator=(initializer_list)", "[assignment]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t dest_size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    const small_vector source{ TestType{ 1 }, TestType{ 2 }, TestType{ 4 } };
    small_vector dest(dest_size, TestType{ 0 });

    dest = { TestType{ 1 }, TestType{ 2 }, TestType{ 4 } };

    REQUIRE(dest.size() == source.size());
    REQUIRE(dest.capacity() >= source.size());
    REQUIRE(dest == source);
}

    //-----------------------------------//
    //             ITERATORS             //
    //-----------------------------------//

TEMPLATE_TEST_CASE("forward iteration", "[iterators]", TrivialType, NonTrivialType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    const small_vector vec(size, TestType{ 1 });

    for (const auto& elem : vec) REQUIRE(elem == TestType{ 1 });
}

TEMPLATE_TEST_CASE("reverse iteration", "[iterators]", TrivialType, NonTrivialType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    const small_vector vec(size, TestType{ 1 });

    for (auto it = vec.rbegin(); it != vec.rend(); ++it) REQUIRE(*it == TestType{ 1 });
}

    //-----------------------------------//
    //           ELEMENT ACCESS          //
    //-----------------------------------//

TEMPLATE_TEST_CASE("operator[]", "[element_access]", TrivialType, NonTrivialType, MoveOnlyType, ImmovableType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    const small_vector<TestType> vec(size);

    REQUIRE(vec[0] == vec[1]);
    REQUIRE(vec[1] == TestType{});
}

TEMPLATE_TEST_CASE("at", "[element_access]", TrivialType, NonTrivialType, MoveOnlyType, ImmovableType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    const small_vector<TestType> vec(size);

    REQUIRE(vec.at(0) == vec.at(1));
    REQUIRE(vec.at(1) == TestType{});
    REQUIRE_THROWS(vec.at(size));
}

TEMPLATE_TEST_CASE("front/back", "[element_access]", TrivialType, NonTrivialType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    small_vector vec(size, TestType{ 2 });

    vec.back() = TestType{ 3 };
    vec.front() = TestType{ 0 };

    REQUIRE(vec.front() == TestType{ 0 });
    REQUIRE(vec.back() == TestType{ 3 });
}

    //-----------------------------------//
    //              CAPACITY             //
    //-----------------------------------//


TEMPLATE_TEST_CASE("is_small", "[capacity]", TrivialType, NonTrivialType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    small_vector vec(size, TestType{ 2 });

    REQUIRE(vec.is_small() == (size == SMALL_SIZE));
}

TEMPLATE_TEST_CASE("reserve", "[capacity]", TrivialType, NonTrivialType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    small_vector vec(size, TestType{ 2 });

    vec.reserve(2 * LARGE_SIZE);
    REQUIRE(vec.capacity() >= 2 * LARGE_SIZE);

    vec.reserve(SMALL_SIZE);
    REQUIRE(vec.capacity() >= 2 * LARGE_SIZE);
}

    //-----------------------------------//
    //             MODIFIERS             //
    //-----------------------------------//

TEMPLATE_TEST_CASE("clear", "[modifiers]", TrivialType, NonTrivialType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    small_vector vec(size, TestType{ 1 });

    vec.clear();

    REQUIRE(vec.empty());
}

TEMPLATE_TEST_CASE("swap", "[modifiers]", TrivialType, NonTrivialType)
{
    const size_t left_size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    const size_t right_size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector left(left_size, TestType{ 1 });
    const small_vector old_left(left);
    small_vector right(right_size, TestType{ 2 });
    const small_vector old_right(right);

    using std::swap;
    swap(left, right);

    REQUIRE(right == old_left);
    REQUIRE(left == old_right);
}

TEMPLATE_TEST_CASE("push_back(const T&)", "[modifiers]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector vec(size, TestType{ 0 });
    const TestType elem(1);

    for (size_t i = 0; i < LARGE_SIZE + 1; i++) vec.push_back(elem);

    REQUIRE(vec.size() == size + LARGE_SIZE + 1);
    REQUIRE(vec.front() == TestType{ 0 });
    REQUIRE(vec.back() == TestType{ 1 });
}

TEMPLATE_TEST_CASE("push_back(T&&)", "[modifiers]", TrivialType, NonTrivialType, MoveOnlyType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector<TestType> vec(size);

    for (size_t i = 0; i < LARGE_SIZE + 1; i++) vec.push_back(TestType{ 1 });

    REQUIRE(vec.size() == size + LARGE_SIZE + 1);
    REQUIRE(vec.front() == TestType{});
    REQUIRE(vec.back() == TestType{ 1 });
}

TEMPLATE_TEST_CASE("emplace_back(...)", "[modifiers]", TrivialType, NonTrivialType, MoveOnlyType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);

    small_vector<TestType> vec(size);

    vec.emplace_back();
    for (size_t i = 0; i < LARGE_SIZE; i++) vec.emplace_back(1);

    REQUIRE(vec.size() == size + LARGE_SIZE + 1);
    REQUIRE(vec.front() == TestType{});
    REQUIRE(vec.back() == TestType{ 1 });
}

TEMPLATE_TEST_CASE("pop_back()", "[modifiers]", TrivialType, NonTrivialType, MoveOnlyType, ImmovableType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    small_vector<TestType> vec(size);

    vec.pop_back();

    REQUIRE(vec.size() == size - 1);
}

TEMPLATE_TEST_CASE("resize(n)", "[modifiers]", TrivialType, NonTrivialType, MoveOnlyType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    small_vector<TestType> vec(size);

    vec.resize(2 * LARGE_SIZE);
    REQUIRE(vec.size() == 2 * LARGE_SIZE);
    REQUIRE(vec.back() == TestType{});

    vec.resize(0);
    REQUIRE(vec.empty());
}

TEMPLATE_TEST_CASE("resize(n, value)", "[modifiers]", TrivialType, NonTrivialType, NonDefaultConstructibleType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    small_vector vec(size, TestType{ 1 });

    vec.resize(2 * LARGE_SIZE, TestType{ 2 });
    REQUIRE(vec.size() == 2 * LARGE_SIZE);
    REQUIRE(vec.back() == TestType{ 2 });

    vec.resize(0, TestType{ 3 });
    REQUIRE(vec.empty());
}

TEMPLATE_TEST_CASE("erase(pos)", "[modifiers]", TrivialType, NonTrivialType, MoveOnlyType)
{
    const size_t size = GENERATE(SMALL_SIZE, LARGE_SIZE);
    small_vector<TestType> vec(size);
    vec.front() = TestType{ 2 };

    vec.erase(vec.begin());
    REQUIRE(vec.size() == size - 1);
    REQUIRE(vec.front() == TestType{});

    vec.erase(vec.end() - 1);
    REQUIRE(vec.size() == size - 2);
}

TEMPLATE_TEST_CASE("erase(first, last)", "[modifiers]", TrivialType, NonTrivialType)
{
    small_vector vec{ TestType{ 0 }, TestType{ 1 }, TestType{ 2 }, TestType{ 3 }, TestType{ 4 } };

    SECTION("front")
    {
        vec.erase(vec.begin(), vec.begin() + 1);
        REQUIRE(vec == small_vector{ TestType{ 1 }, TestType{ 2 }, TestType{ 3 }, TestType{ 4 } });
    }
    SECTION("back")
    {
        vec.erase(vec.end() - 1, vec.end());
        REQUIRE(vec == small_vector{ TestType{ 0 }, TestType{ 1 }, TestType{ 2 }, TestType{ 3 } });
    }
    SECTION("nothing")
    {
        vec.erase(vec.begin(), vec.begin());
        REQUIRE(vec == small_vector{ TestType{ 0 }, TestType{ 1 }, TestType{ 2 }, TestType{ 3 }, TestType{ 4 } });
    }
    SECTION("everything")
    {
        vec.erase(vec.begin(), vec.end());
        REQUIRE(vec.empty());
    }
    SECTION("middle")
    {
        vec.erase(vec.begin() + 1, vec.begin() + 3);
        REQUIRE(vec == small_vector{ TestType{ 0 }, TestType{ 3 }, TestType{ 4 } });
    }
}

TEMPLATE_TEST_CASE("insert(pos, const T&)", "[modifiers]", TrivialType, NonTrivialType)
{
    small_vector vec{ TestType{ 0 }, TestType{ 1 }, TestType{ 2 }, TestType{ 3 } };
    const TestType value{ 21 };

    SECTION("front")
    {
        vec.insert(vec.begin(), value);
        REQUIRE(vec == small_vector{ TestType{ 21 }, TestType{ 0 }, TestType{ 1 }, TestType{ 2 }, TestType{ 3 } });
    }
    SECTION("back")
    {
        vec.insert(vec.end(), value);
        REQUIRE(vec == small_vector{ TestType{ 0 }, TestType{ 1 }, TestType{ 2 }, TestType{ 3 }, TestType{ 21 } });
    }
    SECTION("middle")
    {
        vec.insert(vec.begin() + 2, value);
        REQUIRE(vec == small_vector{ TestType{ 0 }, TestType{ 1 }, TestType{ 21 }, TestType{ 2 }, TestType{ 3 } });
    }
    SECTION("many")
    {
        for (size_t i = 0; i < 100; i++) vec.insert(vec.begin(), value);
        REQUIRE(vec.size() == 104);
        REQUIRE(vec.front() == TestType{ 21 });
        REQUIRE(vec.back() == TestType{ 3 });
    }
}

TEMPLATE_TEST_CASE("insert(pos, T&&)", "[modifiers]", TrivialType, NonTrivialType)
{
    small_vector vec{ TestType{ 0 }, TestType{ 1 }, TestType{ 2 }, TestType{ 3 } };

    SECTION("front")
    {
        vec.insert(vec.begin(), TestType{ 21 });
        REQUIRE(vec == small_vector{ TestType{ 21 }, TestType{ 0 }, TestType{ 1 }, TestType{ 2 }, TestType{ 3 } });
    }
    SECTION("back")
    {
        vec.insert(vec.end(), TestType{ 21 });
        REQUIRE(vec == small_vector{ TestType{ 0 }, TestType{ 1 }, TestType{ 2 }, TestType{ 3 }, TestType{ 21 } });
    }
    SECTION("middle")
    {
        vec.insert(vec.begin() + 2, TestType{ 21 });
        REQUIRE(vec == small_vector{ TestType{ 0 }, TestType{ 1 }, TestType{ 21 }, TestType{ 2 }, TestType{ 3 } });
    }
    SECTION("many")
    {
        for (size_t i = 0; i < 100; i++) vec.insert(vec.begin(), TestType{ 21 });
        REQUIRE(vec.size() == 104);
        REQUIRE(vec.front() == TestType{ 21 });
        REQUIRE(vec.back() == TestType{ 3 });
    }
}

TEMPLATE_TEST_CASE("emplace(pos, ...)", "[modifiers]", TrivialType, NonTrivialType, MoveOnlyType)
{
    small_vector<TestType> vec(2);

    vec.emplace(vec.begin(), 1);
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.front() == TestType{ 1 });

    vec.emplace(vec.end(), 2);
    REQUIRE(vec.size() == 4);
    REQUIRE(vec.back() == TestType{ 2 });

    vec.emplace(vec.begin() + 1);
    REQUIRE(vec.size() == 5);

    for (size_t i = 0; i < 100; i++) vec.emplace(vec.begin());
    REQUIRE(vec.size() == 105);
    REQUIRE(vec.front() == TestType{});
    REQUIRE(vec.back() == TestType{ 2 });
}

