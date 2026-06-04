#include <cat/core/engine.hpp>

#include <cat/core/input_manager.hpp>
#include <cat/util/chrono.hpp>
#include <cat/core/resource_manager.hpp>

using namespace cat;

/*static*/ CoreEngine& CoreEngine::get()
{
    static CoreEngine instance;
    return instance;
}

CoreEngine::CoreEngine()
{
}

CoreEngine::~CoreEngine()
{
}

void CoreEngine::update()
{
    m_chrono.update();
    m_input_manager.update();
}

const Chrono& CoreEngine::get_chrono() const
{
    return m_chrono;
}

const InputManager& CoreEngine::get_input_manager() const
{
    return m_input_manager;
}

ResourceManager& CoreEngine::get_resource_manager()
{
    return m_resource_manager;
}

ECS& CoreEngine::get_ecs()
{
    return m_ecs;
}

#if 0
/*static*/ void Engine::init()
{
    CAT_ASSERT(s_singleton == nullptr);

    s_singleton = new Engine();

	chrono::init();
}

/*static*/ void Engine::quit()
{
    delete s_singleton;
}

/*static*/ void Engine::update()
{
	chrono::update();
}

Engine::Engine()
{
    LOG_TEXT("initing engine\n");
    m_gfx_config = CAT_DEFAULT_GFX_CONFIG;

    CanvasInfo canvas_info = CAT_DEFAULT_WINDOW_CONFIG;
    m_main_window = std::make_unique<SdlCanvas>(
        canvas_info
    );
}


Engine::~Engine()
{
    LOG_TEXT("deleting engine\n");
}
#endif
