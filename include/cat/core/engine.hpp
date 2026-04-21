#pragma once

#include <cat/config.hpp>

namespace cat
{

struct engine
{
public:
	static void init_internals();
	static void update_internals();
	static void destroy_internals();
};

}
