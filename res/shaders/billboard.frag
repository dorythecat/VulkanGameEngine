#version 460

struct PointLight {
    vec4 position;
    vec4 color;
};

layout (set = 0, binding = 0) uniform GlobalUbo {
    mat4 projMatrix;
    mat4 viewMatrix;
    mat4 inverseViewMatrix;

    vec4 ambientLightColor;

    PointLight pointLights[8];
    int pointLightCount;
} globalUbo;

layout (push_constant) uniform PushConstants {
    vec4 position;
    vec4 color;
    float radius;
} push;

layout (location = 0) in vec2 fragOffset;

layout (location = 0) out vec4 outColor;

void main() {
    if(sqrt(dot(fragOffset, fragOffset)) >= 1.0) discard;
    outColor = vec4(push.color.rgb, 1.0);
}