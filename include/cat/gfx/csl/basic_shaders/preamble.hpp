#pragma once

namespace cat::csl
{

inline const char PREAMBLE[] = R"preamble(
    #version 460

    // basic uniforms
    uniform mat4 u_model_matrix;
    uniform mat4 u_view_matrix;
    uniform mat4 u_projection_matrix;

    uniform sampler2D u_texture_atlas;
    
    // vertex hooks
    void pre_process_hook();

    // fragment hooks
    void post_process_hook();
)preamble";

}
