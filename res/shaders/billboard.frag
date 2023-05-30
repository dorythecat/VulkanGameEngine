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

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
} globalUbo;

layout (push_constant) uniform PushConstants {
    vec4 position;
    vec4 color;
    float radius;
} push;

layout (location = 0) in vec2 fragOffset;

layout (location = 0) out vec4 outColor;

const float PI = 3.1415926535;

float cosine(float x) {
    // Which one is actually better?

    // float a = x * x / 2.0;
    // return 1.0 - a * (1.0 - (a / 6.0) * (1.0 - (a / 15.0) * (1.0 - (a / 28.0) * (1.0 - a / 45.0))));

    return cos(x);
}

void main() {
    float dis = sqrt(dot(fragOffset, fragOffset)) * 2.0;
    if (dis >= 1.0) discard;
    outColor = vec4(push.color.rgb, 0.5 * cosine(PI * dis) + 0.5);
}