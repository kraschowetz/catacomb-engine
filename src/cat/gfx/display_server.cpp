#include <cat/gfx/display_server.hpp>

using namespace cat;

DisplayServer* DisplayServer::s_singleton = nullptr;

void DisplayServer::init(void* gl_handle, const GfxConfig &initial_config)
{
    s_singleton = new DisplayServer(gl_handle);
}

void DisplayServer::quit()
{
    delete s_singleton;
}

void DisplayServer::update(const GfxConfig& config)
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

void DisplayServer::prepare(eRenderPass pass)
{
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

DisplayServer::DisplayServer(void* gl_handle)
: m_gl_handle(gl_handle)
{

}
