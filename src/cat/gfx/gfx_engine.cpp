#include "cat/error.hpp"
#include "cat/gfx/render_context.hpp"
#include "cat/gfx/shader.hpp"
#include "cat/gfx/text_renderer.hpp"
#include "cat/util/logger.hpp"
#include "cat/util/memory.hpp"
#include "cat/util/util.hpp"
#include "glaze/core/context.hpp"
#include "glaze/json/read.hpp"
#include "glaze/json/write.hpp"
#include <cat/gfx/gfx_engine.hpp>

#include <cat/gfx/gfx_config.hpp>
#include <cat/gfx/gfx_util.hpp>
#include <cat/gfx/sprite_renderer.hpp>
#include <cat/gfx/texture_loader.hpp>
#include <cat/gfx/shader_loader.hpp>
#include <cat/gfx/csl/csl.hpp>

#include <cat/core/resource_manager.hpp>
#include <cat/core/core_engine.hpp>

#include <glaze/glaze.hpp>
#include <memory>

using namespace cat;

static void _save_config_file(const GfxConfig& config)
{
    glz::error_ctx error = glz::write_file_json(config, CAT_GFX_CONFIG_FILE_PATH, std::string{});

    if(error.ec != glz::error_code::none)
    {
        throw Exception{eErrorCode::FAILED};
    }
}

static GfxConfig _load_config_file()
{
    GfxConfig config; 
    glz::error_ctx error = glz::read_file_json(config, CAT_GFX_CONFIG_FILE_PATH, std::string{});

    if(error.ec != glz::error_code::none)
    {
        LOG_ERR("failed to load gfx_config file! using default settings instead!\n");
        _save_config_file(CAT_DEFAULT_GFX_CONFIG);
        return CAT_DEFAULT_GFX_CONFIG;
    }

    return config;
}

static void _update_gl_state(const GfxConfig& config)
{
    glViewport(0, 0, config.resolution.x, config.resolution.y);
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
    for(u8 i = 0; i < enum_val(eBasicShaderType::COUNT); ++i)
    {
        m_basic_shaders[i] = ShaderLoader{}.load_basic(
            static_cast<eBasicShaderType>(i)
        );
    }
}

void GfxEngine::unload_basic_shaders()
{
    for(u8 i = 0; i < enum_val(eBasicShaderType::COUNT); ++i)
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
    m_main_window = std::make_unique<SdlCanvas>();
    m_sprite_renderer = std::make_unique<SpriteRenderer>();
    m_text_renderer = std::make_unique<TextRenderer>();

    GfxConfig config = _load_config_file();
    _update_gl_state(config);

    load_basic_shaders();
}

GfxEngine::~GfxEngine()
{
    unload_basic_shaders();
}

void GfxEngine::update_settings(const GfxConfig& config)
{
    _update_gl_state(config);
    _save_config_file(config);
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
        case eRenderPass::MAIN_2D:
        case eRenderPass::AUX_2D:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            break;

        case eRenderPass::MAIN_3D:
        case eRenderPass::AUX_3D:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            break;

        case eRenderPass::UI_2D:
        case eRenderPass::UI_3D:
        case eRenderPass::UI_TEXT:
            glDisable(GL_DEPTH_TEST);
            break;

        default: break;
    }

    m_render_context_map.get(pass)->bind();
    m_render_context_map.get(pass)->update();


    m_current_pass = pass;
}

void GfxEngine::finish_render_pass()
{
    switch (m_current_pass) {
        case eRenderPass::MAIN_2D:
        case eRenderPass::AUX_2D:
        case eRenderPass::UI_2D:
            m_sprite_renderer->render_batch();
            break;
        case cat::eRenderPass::UI_TEXT:
            m_text_renderer->render_batch();

        default: break;
    }
}

SpriteRenderer& GfxEngine::get_sprite_renderer()
{
    return *m_sprite_renderer;
}

TextRenderer& GfxEngine::get_text_renderer()
{
    return *m_text_renderer;
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

Watcher<RenderContext> GfxEngine::get_render_context(eRenderPass pass)
{
    ASSERT(m_render_context_map.contains(pass), "accessing null render context");
    ASSERT(m_render_context_map.get(pass) != nullptr, "accessing null render context");

    return m_render_context_map.get(pass);
}

Watcher<RenderContext> GfxEngine::get_current_render_context()
{
    return get_render_context(m_current_pass);
}

Watcher<RenderContext> GfxEngine::create_render_context(
    eRenderPass pass,
    Watcher<Shader> default_shader
)
{
    m_render_context_map.insert(pass, RenderContext{default_shader});
    return m_render_context_map.get(pass);
}
