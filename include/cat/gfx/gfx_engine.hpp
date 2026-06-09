#pragma once

#include "cat/gfx/shader.hpp"
#include "cat/gfx/sprite_renderer.hpp"
#include <cat/gfx/gfx_config.hpp>
#include <cat/gfx/sdl_canvas.hpp>
#include <cat/util/memory.hpp>

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
    PASS_2D,
    PASS_3D,
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

    void display();

private:
    void prepare(eRenderPass pass);
    void finish_render_pass();

private:
    Unique<SdlCanvas> m_main_window;
    Unique<SpriteRenderer> m_sprite_renderer;

    Watcher<Shader> m_default_2d_shader;

    eRenderPass m_current_pass = eRenderPass::NONE;
};

}
