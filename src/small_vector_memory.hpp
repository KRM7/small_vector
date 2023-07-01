#ifndef SMALL_VECTOR_MEMORY_HPP
#define SMALL_VECTOR_MEMORY_HPP

#include "small_vector_traits.hpp"
#include "scope_exit.hpp"
#include <type_traits> // for is_trivially_*, is_nothrow_*
#include <memory> // for allocator_traits, addressof
#include <iterator> // for input_iterator
#include <utility> // for move, forward
#include <cstring> // for memcpy, memset
#include <stdexcept> // for bad_array_new_length

namespace detail
{
    //----------------------------------------- ALLOCATE / DEALLOCATE ---------------------------------------------------

    template<typename T, typename A>
    constexpr alloc_pointer_t<A> allocate(A& allocator, alloc_size_t<A> count)
    {
        if constexpr (is_replaceable_allocator_v<A>)
        {
            auto p = (alloc_pointer_t<A>) std::malloc(sizeof(T) * count);
            if (!p) throw std::bad_alloc{};
            return p;
        }
        else { return std::allocator_traits<A>::allocate(allocator, count); }
    }

    template<typename A>
    constexpr void deallocate(A& allocator, alloc_pointer_t<A> data, alloc_size_t<A> count) noexcept
    {
        if constexpr (is_replaceable_allocator_v<A>)
        {
            std::free(data);
        }
        else { std::allocator_traits<A>::deallocate(allocator, data, count); }
    }

    //--------------------------- CONSTRUCT / DESTROY ONE IN UNINITIALIZED MEMORY ---------------------------------------

    template<typename T, typename A>
    constexpr void destroy(A& allocator, T* at) noexcept
    {
        if constexpr (std::is_trivially_destructible_v<T> && has_trivial_destroy_v<A, T>)
        {
            return; // don't need to do anything
        }
        else { std::allocator_traits<A>::destroy(allocator, at); }
    }

    // default construct
    template<typename T, typename A>
    constexpr void construct(A& allocator, T* at)
    noexcept(std::is_nothrow_default_constructible_v<T> && has_trivial_construct_v<A, T>)
    {
        if constexpr (std::is_trivially_default_constructible_v<T> && has_trivial_construct_v<A, T>)
        {
            std::memset(at, 0, sizeof(T));
        }
        else { std::allocator_traits<A>::construct(allocator, at); }
    }

    // copy consruct
    template<typename T, typename A>
    constexpr void construct(A& allocator, T* at, const T& from)
    noexcept(std::is_nothrow_copy_constructible_v<T> && has_trivial_construct_v<A, T, const T&>)
    {
        if constexpr (std::is_trivially_copy_constructible_v<T> && has_trivial_construct_v<A, T, const T&>)
        {
            std::memcpy(at, std::addressof(from), sizeof(T));
        }
        else { std::allocator_traits<A>::construct(allocator, at, from); }
    }

    // move construct
    template<typename T, typename A>
    constexpr void construct(A& allocator, T* at, T&& from)
    noexcept(std::is_nothrow_move_constructible_v<T> && has_trivial_construct_v<A, T, T&&>)
    {
        if constexpr (std::is_trivially_move_constructible_v<T> && has_trivial_construct_v<A, T, T&&>)
        {
            std::memcpy(at, std::addressof(from), sizeof(T));
        }
        else { std::allocator_traits<A>::construct(allocator, at, std::move(from)); }
    }

    // construct from args
    template<typename T, typename A, typename TArg, typename... TArgs>
    constexpr void construct(A& allocator, T* at, TArg&& arg, TArgs&&... args)
    noexcept(std::is_nothrow_constructible_v<T, TArg, TArgs...> && has_trivial_construct_v<A, TArg, TArgs...>)
    requires(!std::is_same_v<std::remove_cvref_t<TArg>, std::remove_cvref_t<T>>)
    {
        std::allocator_traits<A>::construct(allocator, at, std::forward<TArg>(arg), std::forward<TArgs>(args)...);
    }

    //--------------------------- CONSTRUCT / DESTROY RANGE IN UNINITIALIZED MEMORY ---------------------------------------

    template<typename T, typename A>
    constexpr void destroy_range(A& allocator, T* first, T* last) noexcept
    {
        for (; first != last; ++first) detail::destroy(allocator, first);
    }

    // default construct
    template<typename T, typename A>
    constexpr void construct_range(A& allocator, T* first, T* last)
    noexcept(std::is_nothrow_default_constructible_v<T> && has_trivial_construct_v<A, T>)
    {
        if constexpr (std::is_trivially_default_constructible_v<T> && has_trivial_construct_v<A, T>)
        {
            std::memset(first, 0, sizeof(T) * (last - first));
        }
        else
        {
            T* next = first;
            scope_exit guard{ [&] { detail::destroy_range(allocator, first, next); } };
            for (; next != last; ++next) detail::construct(allocator, next);
            guard.release();
        }
    }

    // copy construct
    template<typename T, typename A>
    constexpr void construct_range(A& allocator, T* first, T* last, const T& val)
    noexcept(noexcept(detail::construct(allocator, first, val)))
    {
        T* next = first;
        scope_exit guard{ [&] { detail::destroy_range(allocator, first, next); } };
        for (; next != last; ++next) detail::construct(allocator, next, val);
        guard.release();
    }

    // construct from range
    template<typename T, typename A, std::forward_iterator Iter>
    constexpr void construct_range(A& allocator, T* first, T* last, Iter src_first)
    noexcept(noexcept(detail::construct(allocator, first, *src_first)))
    {
        using R = std::iter_reference_t<Iter>;

        if constexpr (std::contiguous_iterator<Iter> && std::is_trivially_constructible_v<T, R> && has_trivial_construct_v<A, T, R>)
        {
            std::memcpy(first, std::addressof(*src_first), sizeof(T) * (last - first));
        }
        else
        {
            T* next = first;
            scope_exit guard{ [&] { detail::destroy_range(allocator, first, next); } };
            for (; next != last; ++next, ++src_first) detail::construct(allocator, next, *src_first);
            guard.release();
        }
    }

    // move construct from another range if noexcept
    template<typename T, typename A>
    constexpr void relocate_range_strong(A& allocator, T* first, T* last, T* dest)
    noexcept(std::is_nothrow_move_constructible_v<T> && has_trivial_construct_v<A, T, T&&>)
    {
        if constexpr (is_trivially_relocatable_v<T> && has_trivial_construct_v<A, T, T&&>)
        {
            std::memcpy(dest, first, sizeof(T) * (last - first));
        }
        else
        {
            T* next = dest;
            scope_exit guard{ [&] { detail::destroy_range(allocator, dest, next); } };
            for (; first != last; ++first, ++next) detail::construct(allocator, next, std::move_if_noexcept(*first));
            guard.release();
        }
    }

    // move construct from another range
    template<typename T, typename A>
    constexpr void relocate_range_weak(A& allocator, T* first, T* last, T* dest)
    noexcept(std::is_nothrow_move_constructible_v<T> && has_trivial_construct_v<A, T, T&&>)
    {
        if constexpr (is_trivially_relocatable_v<T> && has_trivial_construct_v<A, T, T&&>)
        {
            std::memcpy(dest, first, sizeof(T) * (last - first));
        }
        else
        {
            T* next = dest;
            scope_exit guard{ [&] { detail::destroy_range(allocator, dest, next); } };
            for (; first != last; ++first, ++next) detail::construct(allocator, next, std::move(*first));
            guard.release();
        }
    }

    //---------------------------------------- ASSIGNMENT METHODS --------------------------------------------------------

    template<typename T, std::forward_iterator Iter>
    constexpr void assign_range(T* first, T* last, Iter src_first)
    noexcept(std::is_nothrow_assignable_v<T&, decltype(*src_first)>)
    {
        if constexpr (std::contiguous_iterator<Iter> && std::is_trivially_assignable_v<T&, decltype(*src_first)>)
        {
            std::memmove(first, std::addressof(*src_first), sizeof(T) * (last - first));
        }
        else
        {
            for (; first != last; ++first, ++src_first) *first = *src_first;
        }
    }

} // namespace detail

#endif // !SMALL_VECTOR_MEMORY_HPP
