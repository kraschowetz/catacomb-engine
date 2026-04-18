#pragma once

#include <type_traits>
#include "cat/config.hpp"		// IWYU pragma: export
#include "cat/error.hpp"		// IWYU pragma: export
#include "cat/util/math.hpp"		// IWYU pragma: export
#include "cat/util/logger.hpp"		// IWYU pragma: export

namespace cat
{

template <typename E>
inline std::underlying_type_t<E> enum_val(E e)
{
	using T = std::underlying_type_t<E>;

	return static_cast<T>(e);
}

}

#define NO_COPY(classname)				\
	classname(const classname&) = delete;		\
	classname& operator=(const classname&) = delete	\
