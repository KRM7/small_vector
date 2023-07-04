#ifndef SMALL_VECTOR_HPP
#define SMALL_VECTOR_HPP

#include "small_vector_traits.hpp" // has_trivial_allocate
#include "small_vector_buffer.hpp" // for small_vector_buffer
#include "small_vector_memory.hpp" // for destroy_range
#include "scope_exit.hpp" // for scope_exit
#include <memory> // for allocator, allocator_traits
#include <new> // for bad_array_new_length
#include <iterator> // for reverse_iterator, iterator concepts, make_move_iterator
#include <initializer_list> // for initializer_list
#include <type_traits> // for is_nothrow_default_constructible
#include <utility> // for move, swap
#include <cstddef> // for size_t, ptrdiff_t
#include <cstdlib> // for calloc
#include <cassert> // for assert
#include <stdexcept> // for out_of_range, bad_alloc
#include <numbers> // for phi
#include <algorithm> // for swap_ranges, equal, lexicographical_compare_three_way


template<typename T, std::size_t Size = 8, typename A = std::allocator<T>>
class small_vector
{
public:
    using value_type      = T;
    using allocator_type  = A;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = typename std::allocator_traits<A>::pointer;
    using const_pointer   = typename std::allocator_traits<A>::const_pointer;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

    using iterator               = T*;
    using const_iterator         = const T*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    //-----------------------------------//
    //            CONSTRUCTORS           //
    //-----------------------------------//

    small_vector() noexcept(std::is_nothrow_default_constructible_v<A>) :
        buffer_(),
        first_(buffer_.begin()),
        last_(first_),
        last_alloc_(buffer_.end()),
        alloc_()
    {}

    explicit small_vector(const A& allocator) noexcept(std::is_nothrow_copy_constructible_v<A>) :
        buffer_(),
        first_(buffer_.begin()),
        last_(first_),
        last_alloc_(buffer_.end()),
        alloc_(allocator)
    {}

    explicit small_vector(size_type count, const A& allocator = {}) :
        buffer_(),
        alloc_(allocator)
    {
        allocate_n(count);
        scope_exit guard{ [&] { deallocate(); } };
        detail::construct_range(alloc_, first_, first_ + count);
        last_ = first_ + count;
        guard.release();
    }

    small_vector(size_type count, const T& value, const A& allocator = {}) :
        buffer_(),
        alloc_(allocator)
    {
        allocate_n(count);
        scope_exit guard{ [&] { deallocate(); } };
        detail::construct_range(alloc_, first_, first_ + count, value);
        last_ = first_ + count;
        guard.release();
    }

    template<std::forward_iterator Iter>
    small_vector(Iter src_first, Iter src_last, const A& allocator = {}) :
        buffer_(),
        alloc_(allocator)
    {
        const auto src_len = std::distance(src_first, src_last);
        allocate_n(src_len);
        scope_exit guard{ [&] { deallocate(); } };
        detail::construct_range(alloc_, first_, first_ + src_len, src_first);
        last_ = first_ + src_len;
        guard.release();
    }

    small_vector(std::initializer_list<T> init, const A& allocator = {}) :
        small_vector(init.begin(), init.end(), allocator)
    {}

    small_vector(const small_vector& other) :
        small_vector(other.begin(), other.end(), std::allocator_traits<A>::select_on_container_copy_construction(other.alloc_))
    {}

    small_vector(const small_vector& other, const A& allocator) :
        small_vector(other.begin(), other.end(), allocator)
    {}

    small_vector(small_vector&& other) noexcept(std::is_trivially_move_constructible_v<T> && has_trivial_construct_v<A, T, T&&>) :
        buffer_(),
        alloc_(std::move(other.alloc_))
    {
        if (other.is_small())
        {
            set_buffer_storage(0);
            detail::relocate_range_weak(alloc_, other.first_, other.last_, first_);
            detail::destroy_range(alloc_, other.first_, other.last_);
            set_buffer_storage(other.size());
            other.last_ = other.first_;
        }
        else
        {
            std::swap(first_, other.first_);
            std::swap(last_, other.last_);
            std::swap(last_alloc_, other.last_alloc_);
        }
    }

    //-----------------------------------//
    //             DESTRUCTOR            //
    //-----------------------------------//

    ~small_vector() noexcept
    {
        detail::destroy_range(alloc_, first_, last_);
        deallocate();
    }

    //-----------------------------------//
    //             ASSIGNMENT            //
    //-----------------------------------//

    template<std::forward_iterator Iter>
    void assign(Iter src_first, Iter src_last)
    {
        const auto src_size = std::distance(src_first, src_last);
        const auto old_size = std::distance(first_, last_);

        if (capacity() >= size_type(src_size))
        {
            if (old_size < src_size)
            {
                detail::assign_range(first_, last_, src_first);
                detail::construct_range(alloc_, last_, first_ + src_size, src_first + old_size);
            }
            else if (old_size >= src_size)
            {
                detail::assign_range(first_, first_ + src_size, src_first);
                detail::destroy_range(alloc_, first_ + src_size, last_);
            }
            last_ = first_ + src_size;
        }
        else if (!is_small() && is_trivially_relocatable_v<T> && is_replaceable_allocator_v<A>)
        {
            pointer new_first = (pointer) std::realloc(first_, sizeof(T) * src_size);
            if (!new_first) throw std::bad_alloc{};
            scope_exit guard{ [&] { std::free(new_first); } };
            detail::assign_range(new_first, new_first + old_size, src_first);
            detail::construct_range(alloc_, new_first + old_size, new_first + src_size, src_first + old_size);
            guard.release();
            set_storage(new_first, src_size, src_size);
        }
        else
        {
            pointer new_first = detail::allocate<T>(alloc_, src_size);
            scope_exit guard{ [&] { detail::deallocate(alloc_, new_first, src_size); } };
            detail::construct_range(alloc_, new_first, new_first + src_size, src_first);
            detail::destroy_range(alloc_, first_, last_);
            guard.release();
            deallocate();
            set_storage(new_first, src_size, src_size);
        }
    }

    small_vector& operator=(const small_vector& other)
    {
        if (std::addressof(other) == this) [[unlikely]] return *this;

        if constexpr (std::allocator_traits<A>::propagate_on_container_copy_assignment::value)
        {
            if (alloc_ != other.alloc_)
            {
                reset();
                alloc_ = other.alloc_;
                allocate_n(other.size());
                detail::construct_range(alloc_, first_, first_ + other.size(), other.first_);
                last_ = first_ + other.size();
                return *this;
            }
        }
        assign(other.begin(), other.end());

        return *this;
    }

    small_vector& operator=(small_vector&& other)
    {
        if (std::addressof(other) == this) [[unlikely]] return *this;

        if (!this->is_small() && !other.is_small())
        {
            swap(other);
            return *this;
        }

        if (this->is_small() && !other.is_small())
        {
            detail::destroy_range(alloc_, first_, last_);

            if constexpr (std::allocator_traits<A>::propagate_on_container_move_assignment::value)
            {
                alloc_ = std::move(other.alloc_);
            }

            this->set_storage(other.first_, other.last_, other.last_alloc_);
            other.set_storage(nullptr, nullptr, nullptr);

            return *this;
        }

        if constexpr (std::allocator_traits<A>::propagate_on_container_move_assignment::value)
        {
            if (alloc_ != other.alloc_)
            {
                reset();
                alloc_ = std::move(other.alloc_);
                allocate_n(other.size());
                detail::relocate_range_weak(alloc_, other.first_, other.last_, first_);
                last_ = first_ + other.size();
                detail::destroy_range(alloc_, other.first_, other.last_);
                other.last_ = other.first_;

                return *this;
            }
        }

        assign(std::make_move_iterator(other.first_), std::make_move_iterator(other.last_));

        return *this;
    }

    small_vector& operator=(std::initializer_list<T> list)
    {
        assign(list.begin(), list.end());
        return *this;
    }

    //-----------------------------------//
    //             ITERATORS             //
    //-----------------------------------//

    iterator begin() noexcept { return first_; }
    const_iterator begin() const noexcept { return first_; }
    const_iterator cbegin() const noexcept { return first_; }

    iterator end() noexcept { return last_; }
    const_iterator end() const noexcept { return last_; }
    const_iterator cend() const noexcept { return last_; }

    reverse_iterator rbegin() noexcept { return std::make_reverse_iterator(last_); }
    const_reverse_iterator rbegin() const noexcept { return std::make_reverse_iterator(last_); }
    const_reverse_iterator crbegin() const noexcept { return std::make_reverse_iterator(last_); }

    reverse_iterator rend() noexcept { return std::make_reverse_iterator(first_); }
    const_reverse_iterator rend() const noexcept { return std::make_reverse_iterator(first_); }
    const_reverse_iterator crend() const noexcept { return std::make_reverse_iterator(first_); }


    //-----------------------------------//
    //           ELEMENT ACCESS          //
    //-----------------------------------//

    reference operator[](size_type i)
    {
        assert(i < size());
        return first_[i];
    }

    const_reference operator[](size_type i) const
    {
        assert(i < size());
        return first_[i];
    }
     
    reference at(size_type i)
    {
        if (i >= size()) throw std::out_of_range{ "Bad vector index." };
        return first_[i];
    }

    const_reference at(size_type i) const
    {
        if (i >= size()) throw std::out_of_range{ "Bad vector index." };
        return first_[i];
    }

    reference front() noexcept { assert(!empty()); return *first_; }
    const_reference front() const noexcept { assert(!empty()); return *first_; }

    reference back() noexcept { assert(!empty()); return *(last_ - 1); }
    const_reference back() const noexcept { assert(!empty()); return *(last_ - 1); }

    pointer data() noexcept { return first_; }
    const_pointer data() const noexcept { return first_; }

    //-----------------------------------//
    //              CAPACITY             //
    //-----------------------------------//

    bool is_small() const noexcept { return first_ == buffer_.begin(); }
    bool empty() const noexcept { return first_ == last_; }

    size_type size() const noexcept { return size_type(last_ - first_); }
    size_type capacity() const noexcept { return size_type(last_alloc_ - first_); }
    size_type max_size() const noexcept { return std::allocator_traits<A>::max_size(alloc_); }

    void reserve(size_type new_capacity) { reallocate_n(new_capacity); }
    void shrink_to_fit() {}

    //-----------------------------------//
    //             MODIFIERS             //
    //-----------------------------------//

    void clear() noexcept
    {
        detail::destroy_range(alloc_, first_, last_);
        last_ = first_;
    }

    void swap(small_vector& other)
    noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_constructible_v<T> && has_trivial_construct_v<A, T, T&&>)
    {
        if (std::addressof(other) == this) [[unlikely]] return;

        if (!this->is_small() && !other.is_small())
        {
            using std::swap;
            swap(first_, other.first_);
            swap(last_, other.last_);
            swap(last_alloc_, other.last_alloc_);
        }
        else if (this->is_small() && other.is_small())
        {
            small_vector& big   = (this->size() < other.size()) ? other : *this;
            small_vector& small = (this->size() < other.size()) ? *this : other;
            const size_type small_size = small.size();

            std::swap_ranges(small.first_, small.last_, big.first_);
            detail::relocate_range_strong(small.alloc_, big.first_ + small.size(), big.last_, small.last_);
            detail::destroy_range(big.alloc_, big.first_ + small.size(), big.last_);

            small.set_buffer_storage(big.size());
            big.set_buffer_storage(small_size);
        }
        else
        {
            small_vector& big   = this->is_small() ? other : *this;
            small_vector& small = this->is_small() ? *this : other;
            const size_type small_size = small.size();

            detail::relocate_range_strong(big.alloc_, small.first_, small.last_, big.buffer_.begin());
            detail::destroy_range(small.alloc_, small.first_, small.last_);

            small.set_storage(big.first_, big.last_, big.last_alloc_);
            big.set_buffer_storage(small_size);
        }

        if constexpr (std::allocator_traits<A>::propagate_on_container_swap::value)
        {
            using std::swap;
            swap(alloc_, other.alloc_);
        }
    }

    void push_back(const T& value) { (void) emplace_back(value); }
    void push_back(T&& value) { (void) emplace_back(std::move(value)); }

    template<typename... Args>
    reference emplace_back(Args&&... args)
    {
        assert(!( memcontains(args) || ... )); // disallowed so we can realloc()

        if (size() == capacity()) reallocate_n(next_capacity());
        detail::construct(alloc_, last_, std::forward<Args>(args)...);
        return *last_++;
    }

    void pop_back() noexcept { assert(!empty()); detail::destroy(alloc_, last_--); }

    void resize(size_type count) { resize_impl(count); }
    void resize(size_type count, const T& value) { resize_impl(count, value); }

    iterator insert(const_iterator pos, const T& value) { return emplace(pos, value); }
    iterator insert(const_iterator pos, T&& value) { return emplace(pos, std::move(value)); }

    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        assert(!( memcontains(args) || ... ));

        if (pos == cend())
        {
            emplace_back(std::forward<Args>(args)...);
            return last_ - 1;
        }

        difference_type offset = std::distance(cbegin(), pos);
        if (size() == capacity()) reallocate_n(next_capacity());
        detail::construct(alloc_, last_, std::move(back()));
        last_++;
        std::move_backward(first_ + offset, last_ - 2, last_ - 1);
        *(first_ + offset) = T(std::forward<Args>(args)...);
        return first_ + offset;
    }

    iterator erase(const_iterator pos) { assert(pos != end()); return erase(pos, pos + 1); }

    iterator erase(const_iterator first, const_iterator last)
    {
        pointer first_erased = first_ + std::distance(cbegin(), first);
        pointer last_erased  = first_ + std::distance(cbegin(), last);
        pointer new_last = std::move(last_erased, last_, first_erased);
        detail::destroy_range(alloc_, new_last, std::exchange(last_, new_last));
        return first_erased;
    }

    //-----------------------------------//
    //               OTHER               //
    //-----------------------------------//

    allocator_type get_allocator() const { return alloc_; }

    friend bool operator==(const small_vector& lhs, const small_vector& rhs) noexcept
    {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    friend auto operator<=>(const small_vector& lhs, const small_vector& rhs) noexcept
    {
        return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

private:

    small_vector_buffer<T, Size> buffer_; // TODO: default buffer size calculations for template param
    pointer first_      = nullptr;
    pointer last_       = nullptr;
    pointer last_alloc_ = nullptr;
    NO_UNIQUE_ADDRESS allocator_type alloc_;

    static inline constexpr double growth_factor_ = std::numbers::phi;


    void allocate_n(size_type count)
    {
        if (count <= buffer_.size())
        {
            set_buffer_storage(0);
        }
        else
        {
            first_ = detail::allocate<T>(alloc_, count);
            last_alloc_ = first_ + count;
        }
    }

    void reallocate_n(size_type new_capacity)
    {
        if (new_capacity <= capacity()) return;

        const size_type old_size = size();

        if (!is_small() && is_trivially_relocatable_v<T> && is_replaceable_allocator_v<A>)
        {
            pointer new_first = (pointer) std::realloc(first_, sizeof(T) * new_capacity);
            if (!new_first) throw std::bad_alloc{};
            set_storage(new_first, old_size, new_capacity);
        }
        else
        {
            pointer new_first = detail::allocate<T>(alloc_, new_capacity);
            scope_exit guard{ [&] { detail::deallocate(alloc_, new_first, new_capacity); } };
            detail::relocate_range_strong(alloc_, first_, last_, new_first);
            detail::destroy_range(alloc_, first_, last_);
            deallocate();
            guard.release();
            set_storage(new_first, old_size, new_capacity);
        }
    }

    void deallocate() noexcept
    {
        if (!is_small() && data()) detail::deallocate(alloc_, first_, capacity());
    }

    void reset() noexcept
    {
        detail::destroy_range(alloc_, first_, last_);
        deallocate();
        set_buffer_storage(0);
    }

    template<typename... Args>
    void resize_impl(size_type count, Args&&... args)
    {
        if (count < size())
        {
            detail::destroy_range(alloc_, first_ + count, last_);
            last_ = first_ + count;
            return;
        }
        else if (count > size())
        {
            reallocate_n(count);
            detail::construct_range(alloc_, last_, first_ + count, std::forward<Args>(args)...);
            last_ = first_ + count;
        }
    }

    void set_storage(pointer first, pointer last, pointer last_alloc) noexcept
    {
        first_ = first;
        last_ = last;
        last_alloc_ = last_alloc;
    }

    void set_storage(pointer first, size_type size, size_type capacity) noexcept
    {
        first_ = first;
        last_ = first + size;
        last_alloc_ = first + capacity;
    }

    void set_buffer_storage(size_type size) noexcept
    {
        first_ = buffer_.begin();
        last_ = buffer_.begin() + size;
        last_alloc_ = buffer_.end();
    }

    size_type next_capacity() const noexcept
    {
        return std::max( capacity() + 1, size_type(growth_factor_ * capacity()) );
    }

    constexpr bool memcontains(const T& elem) const noexcept
    {
        return (first_ <= std::addressof(elem) && std::addressof(elem) < last_);
    }

}; // class small_vector

template<std::forward_iterator Iter, std::size_t Size = 8 /* TODO: default */, typename Alloc = std::allocator<std::iter_value_t<Iter>>>
small_vector(Iter, Iter, Alloc = {}) -> small_vector<std::iter_value_t<Iter>, Size, Alloc>;

template<typename T, std::size_t Size, typename A>
void swap(small_vector<T, Size, A>& lhs, small_vector<T, Size, A>& rhs)
noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

#endif // !SMALL_VECTOR_HPP
