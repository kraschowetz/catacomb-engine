#version 460

layout (location = 0) in vec3 v_color;

out vec4 out_color;

void main()
{
    out_color = vec4(
        v_color.r,
        v_color.g,
        v_color.b,
        1.0
    );
    
    out_color = vec4(1);
}
