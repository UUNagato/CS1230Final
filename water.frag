#version 400 core
in vec3 world_pos;
in vec3 world_normal;
in vec2 uv;
in vec4 screen_pos;
in vec3 local_pos;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

uniform sampler2D shadowMap;
uniform sampler2D cameraDepthTexture;
uniform sampler2D surfaceNoise;
uniform sampler2D surfaceDistortion;
uniform float time;

const int OCTAVES = 1;
const float SWITCH_TIME = 60.f;
out vec4 fragColor;

float rand(vec2 c){
        return fract(sin(dot(c.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

//float noise(vec2 p, float freq ){
//        float unit = 2.f/freq;
//        vec2 ij = floor(p/unit);
//        vec2 xy = mod(p,unit)/unit;
//        xy = 3.*xy*xy-2.*xy*xy*xy;
//        xy = .5*(1.-cos(3.14159265*xy));
//        float a = rand((ij+vec2(0.,0.)));
//        float b = rand((ij+vec2(1.,0.)));
//        float c = rand((ij+vec2(0.,1.)));
//        float d = rand((ij+vec2(1.,1.)));
//        float x1 = mix(a, b, xy.x);
//        float x2 = mix(c, d, xy.x);
//        return mix(x1, x2, xy.y);
//}

//float pNoise(vec2 p, int res){
//        float persistance = 0.5f;
//        float n = 0.f;
//        float normK = 0.f;
//        float f = 100.f;
//        float amp = 100.f;
//        int iCount = 0;
//        for (int i = 0; i<50; i++){
//                n+=amp*noise(p, f);
//                f*=2.;
//                normK+=amp;
//                amp*=persistance;
//                if (iCount == res) break;
//                iCount++;
//        }
//        float nf = n/normK;
//        return nf*nf*nf*nf;
//}

float shadowCalculation(vec4 lightSpacePos)
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    return shadow;
}

float dotNoise2D(in float x, in float y, in float fractionalMaxDotSize, in float dDensity)
{
    float integer_x = x - fract(x);
    float fractional_x = x - integer_x;

    float integer_y = y - fract(y);
    float fractional_y = y - integer_y;

    if (rand(vec2(integer_x+1.0, integer_y +1.0)) > dDensity)
       {return 0.0;}

    float xoffset = (rand(vec2(integer_x, integer_y)) -0.5);
    float yoffset = (rand(vec2(integer_x+1.0, integer_y)) - 0.5);
    float dotSize = 0.5 * fractionalMaxDotSize * max(0.25,rand(vec2(integer_x, integer_y+1.0)));

    vec2 truePos = vec2 (0.5 + xoffset * (1.0 - 2.0 * dotSize) , 0.5 + yoffset * (1.0 -2.0 * dotSize));

    float distance = length(truePos - vec2(fractional_x, fractional_y));

    return 1.0 - smoothstep (0.3 * dotSize, 1.0* dotSize, distance);

}
float DotNoise2D(in vec2 coord, in float wavelength, in float fractionalMaxDotSize, in float dDensity)
{
   return dotNoise2D(coord.x/wavelength, coord.y/wavelength, fractionalMaxDotSize, dDensity);
}


void main(void)
{
    vec4 depthGradientShallow = vec4(0.325f, 0.807f, 0.971f, 0.725f);
    vec4 depthGradientDeep = vec4(0.086f, 0.407f, 1.f, 0.749f);
    float depthMaxDist = 1.f;
    vec4 foamColor = vec4(1.f,1.f,1.f,1.f);
    vec2 surfaceNoiseScroll = vec2(0.05f, 0.05f);
    float surfaceNoiseCutoff = 0.777f;
    float surfaceDistortionAmount = 0.2f;
    float SMOOTHSTEP_AA = 0.01f;

    float depthDifference = length(vec2(world_pos.x, world_pos.z))/2.5f;
    float waterDepthDifference01 = clamp(depthDifference / depthMaxDist, 0, 1);
    vec4 waterColor = mix(depthGradientDeep, depthGradientShallow, waterDepthDifference01);

    vec2 distortSample = (texture(surfaceDistortion, uv).xy * 2 - 1) * surfaceDistortionAmount;

    vec2 noiseUV = vec2(uv.x + time/1000.f * surfaceNoiseScroll.x + distortSample.x, uv.y + time/1000.f*surfaceNoiseScroll.y + distortSample.y);

    vec2 shadowOffset = vec2(0.005, -0.01) * vec2(abs(sin(time / 1000.f))) + vec2(0.05 * distortSample.x, 0.2 * distortSample.y);
    vec3 shadowPos = vec3(local_pos.x + shadowOffset.x, local_pos.y, local_pos.z + shadowOffset.y);
    vec4 lightSpacePoint = lightSpaceMatrix * model * vec4(shadowPos, 1.0);

    float shadow = shadowCalculation(lightSpacePoint);

    float surfaceNoiseSample = texture(surfaceNoise, noiseUV).x;
    // anti-aliasing
    float surfaceNoise = smoothstep(surfaceNoiseCutoff - SMOOTHSTEP_AA, surfaceNoiseCutoff + SMOOTHSTEP_AA, surfaceNoiseSample);

    vec4 surfaceNoiseColor = foamColor;
    surfaceNoiseColor.w *= surfaceNoise * (1 - shadow);
    vec3 color = (surfaceNoiseColor.xyz * surfaceNoiseColor.w) + (waterColor.xyz * (1-surfaceNoiseColor.w));
    float alpha = surfaceNoiseColor.w + waterColor.w*(1-surfaceNoiseColor.w);

    shadow = max(1 - shadow, 0.3);
    fragColor = vec4(shadow * color, alpha);
}
