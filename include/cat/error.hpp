#pragma once

#include "cat/config.hpp"
#include <exception>
#include <stdexcept>
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
	Exception(std::string&& msg) 
		: err_code(std::underlying_type_t<eErrorCode>(eErrorCode::UNKNOWN))
		, message(msg)
	{}
	Exception(eErrorCode err, std::string&& msg) 
		: err_code(std::underlying_type_t<eErrorCode>(err))
		, message(msg)
	{}

protected:
	std::underlying_type_t<eErrorCode> err_code; 
	std::string message;
};
}
