#pragma once

#include <cat/gfx/gfx_util.hpp>     // IWYU pragma: export
#include <cat/gfx/gfx_config.hpp>   // IWYU pragma: export

namespace cat
{

enum class eRenderPass : u8
{
    PASS_2D,
    PASS_3D,
};

class DisplayServer
{
public:
    static void init(void* gl_handle, const GfxConfig& initial_config);
    static void quit();

    inline static DisplayServer& get() { return *s_singleton; }

    void update(const GfxConfig& config);
    void prepare(eRenderPass pass);

    inline const void* get_handle() { return m_gl_handle; };

private:
    DisplayServer(void* gl_handle);

private:
    void* m_gl_handle;

    static DisplayServer* s_singleton;
};

}
