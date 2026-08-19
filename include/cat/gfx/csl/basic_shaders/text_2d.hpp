#pragma once

namespace cat::csl
{

inline constexpr char BASIC_TEXT_2D_VERTEX[] = R"glsl(
    layout (location=0) in vec3 a_position;
    layout (location=1) in vec2 a_uv;

    layout (location=0) out vec3 v_position;
    layout (location=1) out vec2 v_uv;

    void main()
    {
        pre_process_hook();

        vec4 position =
            u_projection_matrix * u_view_matrix * u_model_matrix * vec4(a_position, 1.0f);

        gl_Position = position;

        v_position = position.xyz;
        v_uv = a_uv;
    }
)glsl";

inline constexpr char BASIC_TEXT_2D_FRAGMENT[] = R"glsl(
    layout (location=0) in vec3 v_position;
    layout (location=1) in vec2 v_uv;

    layout (location=0) out vec4 out_color;

    float median(float r, float g, float b)
    {
        return max(min(r, g), min(max(r, g), b));
    }

    void main()
    {
        vec3 msd = texture(u_texture_0, v_uv).rgb;
        float sd = median(msd.r, msd.g, msd.b);

        vec2 unit_range = vec2(u_font_pixel_range) / vec2(textureSize(u_texture_0, 0));
        vec2 screen_tex_size = vec2(1.0) / fwidth(v_uv);
        float screen_pixel_range = max(0.5 * dot(unit_range, screen_tex_size), 1.0);

        float screen_pixel_distance = screen_pixel_range * (sd - 0.5);
        float alpha = clamp(screen_pixel_distance + 0.5, 0.0, 1.0);

        out_color = vec4(u_modulate_color.rgb, u_modulate_color.a * alpha);

        post_process_hook();
    }

)glsl";

}
