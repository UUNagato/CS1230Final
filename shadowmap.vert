#version 400 core
layout (location=0) in vec3 pos;     // Position in object space
layout (location=1) in vec3 normal;
layout (location=2) in vec2 iuv;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main(void)
{
    gl_Position = lightSpaceMatrix * model * vec4(pos, 1.0);
}
