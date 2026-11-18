#include <cat/gfx/sdl_canvas.hpp>

#include "cat/config.hpp"
#include "cat/error.hpp"
#include "cat/gfx/canvas.hpp"
#include "cat/gfx/gfx_util.hpp"
#include "cat/util/logger.hpp"
#include "cat/gfx/gfx_engine.hpp"
#include "glaze/core/context.hpp"
#include "glaze/json/read.hpp"
#include "glaze/json/write.hpp"
#include <SDL2/SDL_video.h>
#include <cat/core/input_manager.hpp>
#include <SDL2/SDL.h>

#include <glaze/glaze.hpp>

namespace cat
{

static void _save_conf_file(const CanvasInfo& info)
{
    glz::error_ctx error = glz::write_file_json(info, CAT_WINDOW_CONFIG_FILE_PATH, std::string{});

    if(error.ec != glz::error_code::none)
    {
        throw Exception{eErrorCode::FAILED};
    }
}

static CanvasInfo _load_canvas_info_file()
{
    CanvasInfo info;
    glz::error_ctx error = glz::read_file_json(info, CAT_WINDOW_CONFIG_FILE_PATH, std::string{});
    
    if(error.ec != glz::error_code::none)
    {
        LOG_ERR("failed to laod window config file! using default config instead!\n");

        _save_conf_file(CAT_DEFAULT_WINDOW_CONFIG);
        return CAT_DEFAULT_WINDOW_CONFIG;
    }

    return info;
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
