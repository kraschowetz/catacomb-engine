#ifndef GFX_H
#define GFX_H

#include "../../include/glad/include/glad/glad.h"	// IWYU pragma: export
#include "../util/util.h"				// IWYU pragma: export
#include <SDL2/SDL.h>					// IWYU pragma: export
#include <SDL2/SDL_opengl.h>				// IWYU pragma: export
#include <GL/glu.h>					// IWYU pragma: export
#include <cglm/cglm.h>					// IWYU pragma: export
#include <cglm/struct.h>				// IWYU pragma: export

/*
* nothing is being rendered (???)
*
* i need to take a few steps back
* here and there... outside this machine even
* then sprint forward
*
* remove that C_Sprite bullshit
* create a raw Triangle @ renderer.c
* then start again from there
*
* update:
* i forgot to call shader_bind() before rendering...
* i wanna kms
*/

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

#endif // GFX_H
