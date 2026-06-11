#include "cat/gfx/shader.hpp"
#include "cat/util/util.hpp"
#include <cat/gfx/gfx_engine.hpp>

#include <cat/gfx/gfx_config.hpp>
#include <cat/gfx/gfx_util.hpp>
#include <cat/gfx/sprite_renderer.hpp>
#include <cat/gfx/texture_loader.hpp>
#include <cat/gfx/shader_loader.hpp>
#include <cat/gfx/csl/csl.hpp>

#include <cat/core/resource_manager.hpp>
#include <cat/core/engine.hpp>

#include <memory>

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

void GfxEngine::load_basic_shaders()
{
    for(u8 i = 0; i < NUM_BASIC_SHADERS; ++i)
    {
        m_basic_shaders[i] = ShaderLoader{}.load_basic(
            static_cast<eBasicShaderType>(i)
        );
    }
}

void GfxEngine::unload_basic_shaders()
{
    for(u8 i = 0; i < NUM_BASIC_SHADERS; ++i)
    {
        ShaderLoader{}.unload(&m_basic_shaders[i]);
    }
}

/*static*/ GfxEngine& GfxEngine::get()
{
    static GfxEngine instance;
    return instance;
}

/*static*/ bool GfxEngine::is_loaded()
{
    // check if opengl ptrs are valid
    // if theyre not, the GfxEngine was probably not loaded
    return glEnable;
}

GfxEngine::GfxEngine()
{
    CanvasInfo window_info = CAT_DEFAULT_WINDOW_CONFIG;

    m_main_window = std::make_unique<SdlCanvas>(window_info);
    m_sprite_renderer = std::make_unique<SpriteRenderer>();
    
    // TODO: load configs from a file
    _update_gl_state(CAT_DEFAULT_GFX_CONFIG);

    load_basic_shaders();
}

GfxEngine::~GfxEngine()
{
    unload_basic_shaders();
}

void GfxEngine::update_settings(const GfxConfig& config)
{
    _update_gl_state(config);

    // TODO: write `config` into a file
}

void GfxEngine::prepare(eRenderPass pass)
{
    if(m_current_pass == eRenderPass::NONE)
    {
        m_main_window->begin_frame();
    }
    else if(m_current_pass != pass)
    {
        finish_render_pass();
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
        default: break;
    }

    m_current_pass = pass;
}

void GfxEngine::finish_render_pass()
{
    switch (m_current_pass) {
        case eRenderPass::PASS_2D:
            m_sprite_renderer->render_batch();
            break;

        default: break;
    }
}

SpriteRenderer& GfxEngine::get_sprite_renderer()
{
    if(m_current_pass != eRenderPass::PASS_2D)
        prepare(eRenderPass::PASS_2D);

    return *m_sprite_renderer;
}

Shader& GfxEngine::get_basic_shader(eBasicShaderType type)
{
    return m_basic_shaders[enum_val(type)];
}

void GfxEngine::display()
{
    finish_render_pass();
    m_main_window->end_frame();
    m_current_pass = eRenderPass::NONE;
}
