#include "small_vector.hpp"

struct Q
{
    Q() = delete;
    Q(int) {};
};

int main()
{
    small_vector<Q> vec0(10, 1);

    small_vector<int> vec1;
    small_vector<int> vec2(std::allocator<int>{});
    small_vector<int> vec3(5);
    small_vector<int> vec4(5, 1);

    std::vector<int> src = { 1, 3, 4 };
    small_vector<int> vec5(src.begin(), src.end());

    small_vector<int> vec6{ 1, 2, 3, 4, 5, 6, 7 };

    vec6.push_back(0);
    vec6.emplace_back(8);


    return vec6 == vec5;

    // iterator ctor

    // copy ctor, move ctor
}