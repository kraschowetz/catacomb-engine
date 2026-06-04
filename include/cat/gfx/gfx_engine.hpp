#pragma once

#include <cat/gfx/gfx_config.hpp>
#include <cat/gfx/sdl_canvas.hpp>
#include <cat/core/memory.hpp>

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
    PASS_2D,
    PASS_3D,
};

class GfxEngine
{
public:
    static GfxEngine& get();

    GfxEngine();
    ~GfxEngine();

    void update_settings(const GfxConfig& config);

    void prepare(eRenderPass pass);
    void display();

private:
    Unique<SdlCanvas> m_main_window;

    bool m_is_rendering = false;
};

}
