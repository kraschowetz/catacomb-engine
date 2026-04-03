#pragma once

#include "cat/config.hpp"
#include <exception>
#include <type_traits>

namespace cat
{
enum class eErrorCode : u32
{
	OK = 0,
	UNKNOWN,
	FAILED,
	FILE_NOT_FOUND,
	OUT_OF_MEMORY,
	OUT_OF_BOUNDS,
	BAD_CAST,
};

class Exception : public std::exception
{
public:
	Exception(eErrorCode err) : err_code(std::underlying_type_t<eErrorCode>(err)) {}
protected:
	std::underlying_type_t<eErrorCode> err_code; };
}
