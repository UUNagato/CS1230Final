#version 400 core
layout (location=0) in vec3 pos;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 iuv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix;
uniform mat4 scaleMatrix;

out vec3 local_pos;
out vec3 world_pos;
out vec3 world_normal;
out vec4 screen_pos;
out vec2 uv;
out vec4 fragPosLightSpace;

void main(void)
{
    local_pos = pos;
    vec4 worldP = model * vec4(pos, 1.0);
    world_pos = vec3(worldP);
    world_normal = vec3(model * vec4(normal, 1.0));
    screen_pos = projection * view * worldP;
    gl_Position = screen_pos;
    fragPosLightSpace = lightSpaceMatrix * worldP;
    uv = mat2(scaleMatrix) * iuv;
}
