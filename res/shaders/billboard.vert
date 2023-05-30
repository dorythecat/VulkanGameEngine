#version 460

const float offsets[12] = {
    -0.5, 0.5,
    0.5, -0.5,
    -0.5, -0.5,
    0.5, 0.5,
    0.5, -0.5,
    -0.5, 0.5,
};

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

    bool texturesEnabled;
} globalUbo;

layout (push_constant) uniform PushConstants {
    vec4 position;
    vec4 color;
    float radius;
} push;

layout (location = 0) out vec2 fragOffset;

void main() {
    fragOffset = vec2(offsets[gl_VertexIndex * 2], offsets[gl_VertexIndex * 2 + 1]);
    vec3 cameraRightWorld = { globalUbo.viewMatrix[0][0], globalUbo.viewMatrix[1][0], globalUbo.viewMatrix[2][0] };
    vec3 cameraUpWorld = { globalUbo.viewMatrix[0][1], globalUbo.viewMatrix[1][1], globalUbo.viewMatrix[2][1] };

    vec3 posWorld = push.position.xyz
    + push.radius * fragOffset.x * cameraRightWorld
    + push.radius * fragOffset.y * cameraUpWorld;

    gl_Position = globalUbo.projMatrix * (globalUbo.viewMatrix * vec4(posWorld, 1.0));
}