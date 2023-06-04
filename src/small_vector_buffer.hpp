#ifndef SMALL_VECTOR_BUFFER_HPP
#define SMALL_VECTOR_BUFFER_HPP

#include <new> // for hardware_constructive_interference_size
#include <cstddef> // for size_t

template<typename T, size_t Size, size_t Align = std::hardware_constructive_interference_size>
struct small_vector_buffer
{
public:
    auto data() noexcept { return reinterpret_cast<T*>(&data_); }
    auto data() const noexcept { return reinterpret_cast<const T*>(&data_); }

    auto begin() noexcept { return reinterpret_cast<T*>(&data_); }
    auto begin() const noexcept { return reinterpret_cast<const T*>(&data_); }

    auto end() noexcept { return reinterpret_cast<T*>(&data_) + Size; }
    auto end() const noexcept { return reinterpret_cast<const T*>(&data_) + Size; }

    constexpr size_t size() const noexcept { return Size; }

private:
    constexpr static size_t align_req = (alignof(T) > Align) ? alignof(T) : Align;
    constexpr static size_t buffer_size = sizeof(T) * Size;

    using storage_type = unsigned char[buffer_size];

    alignas(align_req) storage_type data_;
};

#endif // !SMALL_VECTOR_BUFFER_HPP
