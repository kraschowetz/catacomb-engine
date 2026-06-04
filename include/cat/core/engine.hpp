#pragma once

#include <cat/config.hpp>
#include <cat/core/memory.hpp>
#include <cat/core/ecs.hpp>
#include <cat/core/input_manager.hpp>
#include <cat/core/resource_manager.hpp>
#include <cat/util/chrono.hpp>

enum eEstadoJogador
{
    ANDANDO,
    CORRENDO,
    NO_AR
};

namespace cat
{

class CoreEngine
{
public:
    // brief: get a reference to a global instance of `CoreEngine`
    // this method also creates the global instance on its first call
    static CoreEngine& get();

    CoreEngine();
    ~CoreEngine();

    void update();

    const Chrono& get_chrono() const;
    const InputManager& get_input_manager() const;
    ResourceManager& get_resource_manager();
    ECS& get_ecs();

private:
    Chrono m_chrono;
    InputManager m_input_manager;
    ResourceManager m_resource_manager;
    ECS m_ecs;
};

/*
class CoreEngine
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

    CoreEngine();
    ~CoreEngine();

private:
    GfxConfig m_gfx_config;
    Unique<SdlCanvas> m_main_window;

    static CoreEngine* s_singleton;
};
*/
}
