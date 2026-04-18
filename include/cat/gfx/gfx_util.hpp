#pragma once

#include <glad/glad.h>
#include <cat/config.hpp>
#include <cat/util/logger.hpp>

static inline void gl_clear_all_errors()
{
	while(glGetError() != GL_NO_ERROR) { DEBUGBREAK; }
}

static inline bool gl_check_error_status(const char* fn, u32 line, const char* file)
{
	GLenum err = glGetError();

	if (err != GL_NO_ERROR)
	{
		LOG_ERRF("gl error 0x%u\t line %u\tfunction %s\tfile %s", err, line, fn, file);
		DEBUGBREAK;
		return true;
	}
	return false;
}

#ifdef DEVELOP

#define GL_CALL(_x)							\
	do {								\
		gl_clear_all_errors(); 					\
		_x;							\
		gl_check_error_status(#_x, __LINE__, __FILE__);		\
	} while(false)
#define GL_CHECK_ERROR gl_check_error_status("_", __LINE__, __FILE__)
#else

#define GL_CALL(_x) _x
#define GL_CHECK_ERROR

#endif
