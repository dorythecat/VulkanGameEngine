#version 460

struct PointLight {
    vec4 position;
    vec4 color;
};

layout (set = 0, binding = 0) uniform GlobalUbo {
    mat4 viewMatrix;
    mat4 projMatrix;
    vec4 ambientLightColor;

    PointLight pointLights[8];
    int pointLightCount;
} globalUbo;

layout (push_constant) uniform PushConstant {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPos;
layout (location = 2) in vec3 fragNormal;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 diffuse = globalUbo.ambientLightColor.rgb * globalUbo.ambientLightColor.a;
    vec3 surfaceNormal = normalize(fragNormal);

    for(int i = 0; i < globalUbo.pointLightCount; i++) {
        PointLight light = globalUbo.pointLights[i];
        vec3 directionToLight = light.position.xyz - fragPos;
        float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
        float cosAngIncidence = max(dot(surfaceNormal, normalize(directionToLight)), 0.0);
        vec3 intensity = light.color.xyz * light.color.w * attenuation;

        diffuse += intensity * cosAngIncidence;
    }

    outColor = vec4(diffuse * fragColor, 1.0);
}