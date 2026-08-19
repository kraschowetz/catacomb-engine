#pragma once

#include <glad/glad.h>
#include <cat/config.hpp>
#include <cat/util/logger.hpp>

#ifndef CAT_WINDOW_CONFIG_FILE_PATH

#define CAT_WINDOW_CONFIG_FILE_PATH "./config/window.json"
#define CAT_GFX_CONFIG_FILE_PATH "./config/gfx.json"

#endif

static inline void gl_clear_all_errors()
{
	while(glGetError() != GL_NO_ERROR) { ;; }
}

static inline bool gl_check_error_status(const char* fn, u32 line, const char* file)
{
	GLenum err = glGetError();

	if (err != GL_NO_ERROR)
	{
		LOG_ERR("gl error 0x{}\t line {}\tfunction {}\tfile {}\n", err, line, fn, file);
		DEBUGBREAK;
		return true;
	}
	return false;
}

#ifdef DEVELOP

#define GL_CALL(_x)							                            \
	do {								                                \
		gl_clear_all_errors(); 					                        \
		_x;							                                    \
		gl_check_error_status(#_x, __LINE__, __FILE__);		            \
	} while(false)
#define GL_CHECK_ERROR gl_check_error_status("_", __LINE__, __FILE__)
#else

#define GL_CALL(_x) _x
#define GL_CHECK_ERROR

#endif
