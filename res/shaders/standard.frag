#version 460

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPos;
layout (location = 2) in vec3 fragNormal;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform GlobalUbo {
    mat4 viewMatrix;
    mat4 projMatrix;
    // vec3 lightDir;
    vec4 ambientLightColor;

    vec3 lightPosition;
    vec4 lightColor;
} globalUbo;

layout (push_constant) uniform PushConstant {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main() {
    vec3 lightDir = globalUbo.lightPosition - fragPos;
    float attenuation = 1.0 / dot(lightDir, lightDir);

    vec3 ambientColor = globalUbo.ambientLightColor.rgb * globalUbo.ambientLightColor.a;
    vec3 lColor = globalUbo.lightColor.rgb * globalUbo.lightColor.a * attenuation;

    vec3 diffuseLight = lColor * max(dot(normalize(fragNormal), normalize(lightDir)), 0.0);

    outColor = vec4((diffuseLight + ambientColor) * fragColor, 1.0);
}