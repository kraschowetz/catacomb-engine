#pragma once

#include "cat/gfx/display_server.hpp"
#include <cat/config.hpp>
#include <cat/core/memory.hpp>
#include <cat/gfx/gfx_config.hpp>
#include <cat/gfx/sdl_canvas.hpp>

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

class Engine
{
public:
    static void init();
    static void quit();
    static void update();

    static void render_prepare(eRenderPass pass)
    {
        s_singleton->m_main_window->begin_frame();
    }
    static void display()
    {
        s_singleton->m_main_window->end_frame();
    }

private:
    Engine();
    ~Engine();

private:
    GfxConfig m_gfx_config;
    Unique<SdlCanvas> m_main_window;

    static Engine* s_singleton;
};

}
