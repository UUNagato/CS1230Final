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
uniform sampler2D shadowMap;
uniform bool useTex = true;
uniform vec3 mainColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 fragPosLightSpace;

// controlled via UI
uniform float levels;
uniform float ambientColor;
uniform float material_kd;
uniform float material_ks;
uniform float material_shininess;
uniform vec3 diffuseColor;

const vec3 rimColor = vec3(-1.0f);
const vec3 eye_pos = vec3(0.5f, 3.f, 0.f);
const vec3 light_pos = vec3(4.f, 3.f, 2.f);  // Position of light source

float shadowCalculation(vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    float shadow = 0.0;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - 0.04 > pcfDepth ? 1.0 : 0.0;
        }
    }
    float currentDepth = projCoords.z;
    shadow = shadow / 9.0;
    return shadow;
}

void main(void)
{

//    vec3 n = normalize(cameraNormal);
//    vec3 cameraToVertex = normalize(vertex); //remember we are in camera space!
//    vec3 worldNormal = vec3(inverse(view) * vec4(cameraNormal,0));

//    vec3 L = normalize(-vertexToLight);
//    vec3 V = normalize(-vertexToCamera);

    vec3 diffColor = diffuseColor;

    float scaleFactor = 1.0 / levels;

    vec3 L = normalize( light_pos - world_pos);
    vec3 V = normalize( eye_pos - world_pos);

    float diffuse = max(0, dot(L, world_normal));

    vec3 color = useTex ? texture(mainTex, uv).xyz : mainColor;
//    diffColor = texture(mainTex, uv).xyz; // use this or constant color

    diffColor = diffColor * material_kd * floor(diffuse * levels) * scaleFactor;

    vec3 H = normalize(L + V);
    float specular = 0.0;

    if ( dot(L, world_normal) > 0.0) {
        specular = material_ks * pow( max(0, dot(H, world_normal)), material_shininess);
    }

    float specMask = (pow(dot(H, world_normal), material_shininess) > 0.4) ? 1: 0;
    float edgeDetection = (dot(V, world_normal) > 0.15) ? 1 : 0;
    edgeDetection = 1;

    float shadow = shadowCalculation(fragPosLightSpace);
    shadow = max(1 - shadow, 0.2);

    color = ambientColor + edgeDetection * (color + diffColor + specular * specMask);


    ///////// EXP
//    float edgeDetection = (dot(V, world_normal) > 0.4) ? 0 : dot(V, world_normal);
//    float edgeDetection = 1.0f;
//    color = ambientColor + (color + diffuseColor + specular * specMask) - edgeDetection * rimColor;
    ///////// end EXP

    fragColor = vec4(shadow * color, 1.0);
}
