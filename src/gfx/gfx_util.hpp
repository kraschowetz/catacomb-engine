#ifndef GFX_UTIL_H
#define GFX_UTIL_H

#include "../../include/glad/include/glad/glad.h"	// IWYU pragma: export
#include "../util/util.hpp"				// IWYU pragma: export
#include <SDL2/SDL.h>					// IWYU pragma: export
#include <SDL2/SDL_opengl.h>				// IWYU pragma: export
#include <GL/glu.h>					// IWYU pragma: export
#include <glm/glm.hpp>					// IWYU pragma: export

#include <array>

#define GFX_QUAD_RENDERING_ORDER {0, 1, 2, 2, 3, 0}

static inline void _gl_clear_all_errors() {
	while(glGetError() != GL_NO_ERROR) {}
}

static inline bool _gl_check_error_status(const char *function, u32 line, const char *file) {
	GLenum err = glGetError();
	while(err) {
		printf("GL ERROR %u\tline: %u\tfunction: %s\tfile: %s\n", err, line, function, file);
		return true;
	}
	return false;
}

#define GL_CHECK(x) _gl_clear_all_errors(); x; _gl_check_error_status(#x, __LINE__, __FILE__);

template <u64 Size>
static constexpr std::array<u32, Size * 6> gen_quad_batch_ids() {
	std::array<u32, Size * 6> ids;
	for(u32 i = 0; i < Size; i++) {
		ids[i*6] = i*4;
		ids[(i*6)+1] = i*4+1;
		ids[(i*6)+2] = i*4+2;
		ids[(i*6)+3] = i*4+2;
		ids[(i*6)+4] = i*4+3;
		ids[(i*6)+5] = i*4;
	}

	return ids;
}

#endif // GFX_UTIL_H
