#ifndef UTIL_H
#define UTIL_H

#include <iostream>	// IWYU pragma: export
#include <cstdlib>	// IWYU pragma: export
#include <type_traits>
#include "types.hpp"	// IWYU pragma: export

#define THROW_ERR(_err) {std::cerr << _err << "\n"; std::abort();}

#if NO_ASSERTS
#define ASSERT(_expr, msg)
#else
#define ASSERT(_expr, _msg) 		\
	if(!(_expr)) { 			\
		std::cerr << _msg; 	\
		std::abort();		\
	}
#endif

template <typename Enum>
inline constexpr auto enum_val(Enum val) {
	return static_cast<std::underlying_type<Enum>::type>(val);
}

#endif // UTIL_H
