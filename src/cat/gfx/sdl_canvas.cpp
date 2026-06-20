#include <cat/gfx/sdl_canvas.hpp>

#include "cat/config.hpp"
#include "cat/error.hpp"
#include "cat/gfx/canvas.hpp"
#include "cat/gfx/gfx_util.hpp"
#include "cat/util/logger.hpp"
#include "cat/gfx/gfx_engine.hpp"
#include <SDL2/SDL_video.h>
#include <cat/core/input_manager.hpp>
#include <SDL2/SDL.h>

#include <cat/util/cconf.hpp>

namespace cat
{

static CanvasInfo _load_canvas_info_file()
{
    try {
        BasicConfMap map = load_conf_file(CAT_WINDOW_CONFIG_FILE_PATH);

        CanvasInfo info;
        info.height = std::get<u32>(map.at("height").data);
        info.width = std::get<u32>(map.at("width").data);
        info.title = std::get<std::string>(map.at("title").data);
        info.version.major = std::get<u32>(map.at("gl.major").data);
        info.version.minor = std::get<u32>(map.at("gl.minor").data);

        return info;
    } 
    catch (Exception e) {
        // TODO: write windowconf file
        return CAT_DEFAULT_WINDOW_CONFIG;
    }
}

SdlCanvas::SdlCanvas()
{
    CanvasInfo info = _load_canvas_info_file();

	SDL_Init(SDL_INIT_EVERYTHING);
	m_sdl_handle = SDL_CreateWindow(
		info.title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		static_cast<i32>(info.width),
		static_cast<i32>(info.height),
		SDL_WINDOW_OPENGL
	);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, static_cast<i32>(info.version.major));
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, static_cast<i32>(info.version.minor));
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_gl_handle = SDL_GL_CreateContext(m_sdl_handle);

    // set vsync by default only in release mode
    // TODO: handle vsync dynamically
#ifdef DEVELOP
	SDL_GL_SetSwapInterval(0);
#else
	SDL_GL_SetSwapInterval(1);
#endif
	
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
    SDL_GL_SwapWindow(m_sdl_handle);
}

void SdlCanvas::poll_events()
{
}

}
