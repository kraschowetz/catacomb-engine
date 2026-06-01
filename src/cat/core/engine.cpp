#include "cat/config.hpp"
#include "cat/gfx/canvas.hpp"
#include <cat/core/engine.hpp>

#include <cat/core/input_manager.hpp>
#include <cat/util/chrono.hpp>
#include <cat/core/resource_manager.hpp>
#include <memory>

using namespace cat;

Engine* Engine::s_singleton = nullptr;

/*static*/ void Engine::init()
{
    CAT_ASSERT(s_singleton == nullptr);

    s_singleton = new Engine();

	chrono::init();
	input::init();
}

/*static*/ void Engine::quit()
{
	input::quit();

    delete s_singleton;
}

/*static*/ void Engine::update()
{
	input::update();
	chrono::update();
}

Engine::Engine()
{
    m_gfx_config = CAT_DEFAULT_GFX_CONFIG;

    CanvasInfo canvas_info = CAT_DEFAULT_WINDOW_CONFIG;
    m_main_window = std::make_unique<SdlCanvas>(
        canvas_info
    );
}

Engine::~Engine()
{
}
