#version 400 core
in vec3 world_pos;
in vec3 world_normal;
in vec2 uv;
in vec4 fragPosLightSpace;

uniform sampler2D shadowMap;

out vec4 fragColor;

float shadowCalculation(vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main(void)
{
    // float shadow = shadowCalculation(fragPosLightSpace);
    float shadow = shadowCalculation(fragPosLightSpace);
    fragColor = (1.0 - shadow) * vec4(0.0, 0.0, 1.0, 1.0);
}
