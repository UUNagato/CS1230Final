#version 400 core

layout (location=0) in vec3 pos;     // Position in object space
layout (location=1) in vec3 normal;
layout (location=2) in vec2 iuv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 world_pos;
out vec3 world_normal;
out vec2 uv;

out vec3 vertex;                    // Vertex pos in camera space
out vec3 vertexToLight;             // Vector from vertex to light
out vec3 vertexToCamera;            // Vector from vertex to camera
out vec3 cameraNormal;              // Normal in camera space
out vec4 fragPosLightSpace;


// z is up and down(-down), y is left and right (-right), x is near far(-far)
const vec4 lightPosition = vec4(0, -50, 20, 0);  // Position of light source

void main(void)
{
    vec4 worldP = model * vec4(pos, 1.0);
    fragPosLightSpace = lightSpaceMatrix * worldP;
    world_pos = vec3(worldP);
    world_normal = vec3(model * vec4(normal, 0));
    gl_Position = projection * view * worldP;
    uv = iuv;

    vertex = ((view * model) * (vec4(pos, 1.0))).xyz;
    vec4 cameraLightPosition = view * model * lightPosition;           // Light position in camera space
    vertexToLight = normalize(cameraLightPosition.xyz - vertex);
    vertexToCamera = -normalize(vertex);
    cameraNormal = normalize(mat3(transpose(inverse(view*model))) * normal);


}
