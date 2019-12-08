#version 400 core
in vec3 world_pos;
in vec3 world_normal;
in vec2 uv;

in vec3 vertex;                    // Vertex pos in camera space
in vec3 vertexToLight;             // Vector from vertex to light
in vec3 vertexToCamera;            // Vector from vertex to camera
in vec3 cameraNormal;              // Normal in camera space

out vec4 fragColor;

uniform sampler2D mainTex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// to be updated
//vec3 diffuseColor = vec3(0.30, 0.80, 0.10);
vec3 diffuseColor = vec3(0.30, 0.80, 0.10);

const vec3 ambientColor = vec3(0.01f);
const float material_kd = 1.0f;
const float material_ks = 0.2f;
const float material_shininess = 0.9;
const float levels = 9.0;
const float scaleFactor = 1.0 / levels;

const vec3 rimColor = vec3(-1.0f);
const vec3 eye_pos = vec3(0.5f, 3.f, 0.f);
const vec3 light_pos = vec3(4.f, 3.f, 2.f);  // Position of light source

void main(void)
{

//    vec3 n = normalize(cameraNormal);
//    vec3 cameraToVertex = normalize(vertex); //remember we are in camera space!
//    vec3 worldNormal = vec3(inverse(view) * vec4(cameraNormal,0));

//    vec3 L = normalize(-vertexToLight);
//    vec3 V = normalize(-vertexToCamera);

    vec3 L = normalize( light_pos - world_pos);
    vec3 V = normalize( eye_pos - world_pos);

    float diffuse = max(0, dot(L, world_normal));

    vec3 color = texture(mainTex, uv).xyz;
    diffuseColor = texture(mainTex, uv).xyz; // use this or constant color

    diffuseColor = diffuseColor * material_kd * floor(diffuse * levels) * scaleFactor;

    vec3 H = normalize(L + V);
    float specular = 0.0;

    if ( dot(L, world_normal) > 0.0) {
        specular = material_ks * pow( max(0, dot(H, world_normal)), material_shininess);
    }

    float specMask = (pow(dot(H, world_normal), material_shininess) > 0.4) ? 1: 0;
    float edgeDetection = (dot(V, world_normal) > 0.15) ? 1 : 0;

    color = ambientColor + edgeDetection * (color + diffuseColor + specular * specMask);


    ///////// EXP
//    float edgeDetection = (dot(V, world_normal) > 0.4) ? 0 : dot(V, world_normal);
//    float edgeDetection = 1.0f;
//    color = ambientColor + (color + diffuseColor + specular * specMask) - edgeDetection * rimColor;
    ///////// end EXP


    fragColor= vec4(pow(color, vec3(1.0 / 2.2)), 1);

}
