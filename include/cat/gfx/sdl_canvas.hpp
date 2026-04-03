#pragma once

#include <cat/gfx/canvas.hpp>
#include <SDL2/SDL.h>

namespace cat
{

class SdlCanvas : public iCanvas
{
public:
	virtual void begin_frame() override;
	virtual void end_frame() override;
	virtual void poll_events() override;

	SdlCanvas(CanvasInfo& info);
	~SdlCanvas();

private:
	SDL_Window* m_sdl_handle;
	void* m_gl_handle;
	CanvasInfo m_info;
};

}
