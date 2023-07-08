#ifndef SMALL_VECTOR_SCOPE_EXIT_HPP
#define SMALL_VECTOR_SCOPE_EXIT_HPP

#include <functional>
#include <type_traits>
#include <utility>

template<typename F>
class [[nodiscard]] scope_exit
{
public:
    constexpr explicit scope_exit(F on_exit)
    noexcept(std::is_nothrow_move_constructible_v<F>) :
        on_exit_(std::move(on_exit))
    {}

    scope_exit(const scope_exit&)            = delete;
    scope_exit(scope_exit&&)                 = delete;
    scope_exit& operator=(const scope_exit&) = delete;
    scope_exit& operator=(scope_exit&&)      = delete;

    constexpr ~scope_exit() noexcept
    {
        if (active_) [[unlikely]] std::invoke(std::move(on_exit_));
    }

    constexpr void release() noexcept { active_ = false; }

private:
    F on_exit_;
    bool active_ = true;
};

#endif // !SMALL_VECTOR_SCOPE_EXIT_HPP
