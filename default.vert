#version 400 core

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 fragPos;

//uniform mat4 mvp;
//uniform mat4 model;

void main(void) {
//    fragPos = (model * vec4(position, 1)).xyz;
//    vec4 pos = mvp * vec4(position, 1);
    gl_Position = vec4(position, 1.0);
}
