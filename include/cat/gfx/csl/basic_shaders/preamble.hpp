#pragma once

namespace cat::csl
{

inline const char PREAMBLE[] = R"preamble(
    #version 460

    // basic uniforms
    uniform mat4 u_model_matrix;
    uniform mat4 u_view_matrix;
    uniform mat4 u_projection_matrix;

    uniform sampler2D u_texture_0;
    uniform sampler2D u_texture_1;
    uniform sampler2D u_texture_2;
    uniform sampler2D u_texture_3;

    uniform vec4 u_modulate_color = vec4(1.0);

    // text-specific uniforms
    uniform float u_font_pixel_range;
    
    // vertex hooks
    void pre_process_hook();

    // fragment hooks
    void post_process_hook();
)preamble";

}
