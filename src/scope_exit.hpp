#ifndef SCOPE_EXIT_HPP
#define SCOPE_EXIT_HPP

#include <functional> // for invoke
#include <type_traits> // for is_nothrow_move_constructible
#include <utility> // for move

template<typename F>
class [[nodiscard]] scope_exit
{
public:
    constexpr explicit scope_exit(F on_exit)
    noexcept(std::is_nothrow_move_constructible_v<F>) :
        on_exit_(std::move(on_exit))
    {}

    constexpr scope_exit(scope_exit&& other)
    noexcept(std::is_nothrow_move_constructible_v<F>) :
        on_exit_(std::move(other.on_exit_)),
        active_(other.active_)
    {
        other.release();
    }

    scope_exit(const scope_exit& other)            = delete;
    scope_exit& operator=(const scope_exit& other) = delete;
    scope_exit& operator=(scope_exit&& other)      = delete;

    constexpr ~scope_exit() noexcept
    {
        if (active_) [[unlikely]] std::invoke(std::move(on_exit_));
    }

    constexpr void release() noexcept { active_ = false; }

private:
    F on_exit_;
    bool active_ = true;
};

#endif // !SCOPE_EXIT_HPP
