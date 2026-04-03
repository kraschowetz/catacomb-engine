#pragma once

#include <type_traits>
namespace cat
{

template <typename E>
inline std::underlying_type_t<E> enum_val(E e)
{
	using T = std::underlying_type_t<E>;

	return static_cast<T>(e);
}

}
