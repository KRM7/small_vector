#ifndef SMALL_VECTOR_TRAITS_HPP
#define SMALL_VECTOR_TRAITS_HPP

#include <type_traits>
#include <memory>
#include <utility>


#ifndef SMALL_VECTOR_KEEP_TRIVIAL_ALLOCATOR
	inline constexpr bool replace_trivial_allocators = true;
#else
	inline constexpr bool replace_trivial_allocators = false;
#endif


// 'Allocator' has a trivial construct method for the type 'T' with the arguments 'TArgs'
// if calling allocator_traits<Allocator>::construct is equivalent to directly calling the
// construct of T
template<typename Allocator, typename T, typename... Args>
struct has_trivial_construct
{
private:
	template<typename = decltype( std::declval<Allocator>().construct(std::declval<T*>(), std::declval<Args>()...) )>
	static std::false_type test(int);
	static std::true_type test(...);
public:
	constexpr static bool value = decltype( test(0) )::value;
};

template<typename Allocator, typename T, typename... Args>
inline constexpr bool has_trivial_construct_v = has_trivial_construct<Allocator, T, Args...>::value;


// 'Allocator' has a trivial destroy method for the type 'T' if calling
// allocator_traits<Allocator>::destroy is equivalent to directly calling the
// destructor of T
template<typename Allocator, typename T>
struct has_trivial_destroy
{
private:
	template<typename = decltype( std::declval<Allocator>().destroy(std::declval<T*>()) )>
	static std::false_type test(int);
	static std::true_type test(...);
public:
	constexpr static bool value = decltype( test(0) )::value;
};

template<typename Allocator, typename T>
inline constexpr bool has_trivial_destroy_v = has_trivial_destroy<Allocator, T>::value;


// 'Allocator' is considered trivial if its allocate and deallocate methods are equivalent
// to calling global operator new[] and delete[] with unspecified alginments.
template<typename Allocator>
struct is_trivial_allocator : std::false_type {};

template<typename T>
struct is_trivial_allocator<std::allocator<T>> : std::true_type {};

template<typename Allocator>
inline constexpr bool is_trivial_allocator_v = is_trivial_allocator<Allocator>::value;


// Allocators whose allocation methods can be replaced with malloc() and free().
template<typename Allocator>
struct is_replaceable_allocator :
	std::bool_constant<is_trivial_allocator_v<Allocator> && replace_trivial_allocators>
{};

template<typename Allocator>
inline constexpr bool is_replaceable_allocator_v = is_replaceable_allocator<Allocator>::value;


// Types for which move construction and assignment can be replaced with memcpy()/memmove().
template<typename T>
struct is_trivially_relocatable :
	std::conjunction<std::is_trivially_move_constructible<T>, std::is_trivially_move_assignable<T>, std::is_trivially_destructible<T>>
{};

template<typename T>
inline constexpr bool is_trivially_relocatable_v = is_trivially_relocatable<T>::value;

#endif // !SMALL_VECTOR_TRAITS_HPP
