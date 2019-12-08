#version 400 core
layout (location=0) in vec3 pos;     // Position in object space
layout (location=1) in vec3 normal;
layout (location=2) in vec2 iuv;
const float offset = 0.05;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 world_pos;
out vec3 world_normal;

void main(void)
{
    vec4 worldP = model * vec4(pos, 1.0);
    world_pos = worldP.xyz;

    world_normal = vec3(model * vec4(normal, 0.0));
    world_pos += world_normal * offset;

    gl_Position = projection * view * vec4(world_pos, 1.0);
}
