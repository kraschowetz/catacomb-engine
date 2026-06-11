
#pragma once

namespace cat::csl
{

inline constexpr char BASIC_UNLIT_2D_FRAGMENT[] = R"std_fragment_2d(
    layout (location=0) in vec3 v_position;
    layout (location=1) in vec2 v_uv;

    uniform sampler2D u_sprite_atlas;

    layout (location=0) out vec4 out_color;

    void main()
    {
        out_color = texture(u_sprite_atlas, v_uv);

        post_process_hook();
    }

)std_fragment_2d";

inline constexpr char BASIC_UNLIT_2D_VERTEX[] = R"std_vertex_2d(
    layout (location=0) in vec3 a_position;
    layout (location=1) in vec2 a_uv;

    layout (location=0) out vec3 v_position;
    layout (location=1) out vec2 v_uv;

    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_perspective;

    void main()
    {
        vec4 position = u_perspective * u_view * u_model * vec4(a_position, 1.0f);   

        gl_Position = vec4(
            position.x,
            position.y,
            position.z,
            position.w
        );

        gl_Position = vec4(
            a_position.x / 800,
            a_position.y / 600,
            0,
            1
        );

        // who knowns who might use this...
        v_position = position.xyz;
        
        v_uv = a_uv;
    }

)std_vertex_2d";

}
