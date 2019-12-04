#version 400 core
in vec3 world_pos;
in vec3 world_normal;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D mainTex;

void main(void)
{
    fragColor = texture(mainTex, uv);
}
