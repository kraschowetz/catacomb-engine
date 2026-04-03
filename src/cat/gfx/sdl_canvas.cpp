#include <cat/gfx/sdl_canvas.hpp>

#include "cat/config.hpp"
#include "cat/gfx/gfx_util.hpp"
#include "cat/util/logger.hpp"
#include <SDL2/SDL_video.h>
#include <cat/core/input_manager.hpp>
#include <SDL2/SDL.h>

namespace cat
{

SdlCanvas::SdlCanvas(CanvasInfo& info)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	m_sdl_handle = SDL_CreateWindow(
		info.title.data(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		static_cast<i32>(info.width),
		static_cast<i32>(info.height),
		SDL_WINDOW_OPENGL
	);

	m_gl_handle = SDL_GL_CreateContext(m_sdl_handle);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, static_cast<i32>(info.version.major));
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, static_cast<i32>(info.version.minor));
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_GL_SetSwapInterval(1);
	
	if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		LOG_ERR("failed to laod opengl!");
		DEBUGBREAK;
	}

	m_info = info;
}

SdlCanvas::~SdlCanvas()
{
	SDL_GL_DeleteContext(m_gl_handle);
	SDL_DestroyWindow(m_sdl_handle);
	SDL_Quit();
}

void SdlCanvas::begin_frame()
{
}

void SdlCanvas::end_frame()
{
}

void SdlCanvas::poll_events()
{
}

}
