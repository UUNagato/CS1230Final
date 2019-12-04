#version 400 core
in vec3 world_pos;
in vec3 world_normal;
in vec2 uv;

out vec4 fragColor;

void main(void)
{
    fragColor = vec4(0.1, 0.1, 0.8, 1.0);
}
