#include "cat/gfx/gfx_config.hpp"
#include <cat/gfx/gfx_engine.hpp>

#include <cat/gfx/gfx_util.hpp>

using namespace cat;

static void _update_gl_state(const GfxConfig& config)
{
    glViewport(0, 0, 800, 600);
    glClearColor(
        config.clear_color.r,
        config.clear_color.g,
        config.clear_color.b,
        config.clear_color.a
    );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_TEXTURE_CUBE_MAP);  // futureproofing for 3D games
}

/*static*/ GfxEngine& GfxEngine::get()
{
    static GfxEngine instance;
    return instance;
}

GfxEngine::GfxEngine()
{
    CanvasInfo window_info = CAT_DEFAULT_WINDOW_CONFIG;

    m_main_window = std::make_unique<SdlCanvas>(window_info);
    
    // TODO: load configs from a file
    _update_gl_state(CAT_DEFAULT_GFX_CONFIG);
}

GfxEngine::~GfxEngine()
{
}

void GfxEngine::update_settings(const GfxConfig& config)
{
    _update_gl_state(config);

    // TODO: write `config` into a file
}

void GfxEngine::prepare(eRenderPass pass)
{
    if(!m_is_rendering)
    {
        m_is_rendering = true;
        m_main_window->begin_frame();
    }

    switch(pass)
    {
        case eRenderPass::PASS_2D:
            glDisable(GL_DEPTH_TEST);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            break;

        case eRenderPass::PASS_3D:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            break;
    }
}

void GfxEngine::display()
{
    m_main_window->end_frame();
    m_is_rendering = false;
}
