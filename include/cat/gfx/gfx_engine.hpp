#pragma once

#include "cat/gfx/render_context.hpp"
#include "cat/gfx/shader.hpp"
#include "cat/gfx/sprite_renderer.hpp"
#include "cat/gfx/text_renderer.hpp"
#include <cat/gfx/gfx_config.hpp>
#include <cat/gfx/sdl_canvas.hpp>
#include <cat/util/memory.hpp>
#include <cat/util/stable_buffer.hpp>
#include <cat/util/dense_map.hpp>

#ifndef CAT_DEFAULT_GFX_CONFIG
    #define CAT_DEFAULT_GFX_CONFIG                  \
        GfxConfig {                                 \
            .clear_color = {0.f, 0.f, 0.f, 1.f},    \
            .resolution = {800, 600}                \
        }
#endif

#ifndef CAT_DEFAULT_WINDOW_CONFIG
    #define CAT_DEFAULT_WINDOW_CONFIG   \
        CanvasInfo {                    \
            .title = "catacomb engine", \
            .width = 800,               \
            .height = 600,              \
            .version = {4, 6}           \
        }
#endif

namespace cat
{

enum class eRenderPass : u8
{
    NONE = 0,
    MAIN_2D,
    MAIN_3D,
    UI_2D,
    UI_3D,
    UI_TEXT,
    AUX_2D,
    AUX_3D
};

class GfxEngine
{
public:
    static GfxEngine& get();
    static bool is_loaded();

    GfxEngine();
    ~GfxEngine();

    void update_settings(const GfxConfig& config);

    SpriteRenderer& get_sprite_renderer();
    TextRenderer& get_text_renderer();
    Shader& get_basic_shader(eBasicShaderType type);

    // @return ptr to a render pass' render context
    Watcher<RenderContext> get_render_context(eRenderPass pass);
    Watcher<RenderContext> get_current_render_context();

    Watcher<RenderContext> create_render_context(
        eRenderPass pass, 
        Watcher<Shader> default_shader
    );

    void prepare(eRenderPass pass);
    void display();

public:
    constexpr static hash_t MAIN_2D_CONTEXT = 0;
    constexpr static hash_t MAIN_3D_CONTEXT = 1;

private:
    void finish_render_pass();

    void load_basic_shaders();
    void unload_basic_shaders();

private:
    Unique<SdlCanvas> m_main_window;
    Unique<SpriteRenderer> m_sprite_renderer;
    Unique<TextRenderer> m_text_renderer;

    Weak<Shader> m_current_shader;

    std::array<Shader, 4> m_basic_shaders;
    DenseMap<eRenderPass, RenderContext> m_render_context_map;

    eRenderPass m_current_pass = eRenderPass::NONE;
};

}
