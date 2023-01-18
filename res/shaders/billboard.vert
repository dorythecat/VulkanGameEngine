#version 460

const float offsets[12] = {
    -0.5, -0.5,
    0.5, -0.5,
    -0.5, 0.5,
    -0.5, 0.5,
    0.5, -0.5,
    0.5, 0.5,
};

layout (location = 0) out vec2 fragOffset;

layout (set = 0, binding = 0) uniform GlobalUbo {
    mat4 viewMatrix;
    mat4 projMatrix;
    // vec3 lightDir;
    vec4 ambientLightColor;

    vec3 lightPosition;
    vec4 lightColor;
} globalUbo;

const float radius = 1;

void main() {
    fragOffset = vec2(offsets[gl_VertexIndex * 2], offsets[gl_VertexIndex * 2 + 1]);
    vec3 cameraRightWorld = {globalUbo.viewMatrix[0][0], globalUbo.viewMatrix[1][0], globalUbo.viewMatrix[2][0]};
    vec3 cameraUpWorld = {globalUbo.viewMatrix[0][1], globalUbo.viewMatrix[1][1], globalUbo.viewMatrix[2][1]};

    vec3 posWorld = globalUbo.lightPosition.xyz
    + radius * fragOffset.x * cameraRightWorld
    + radius * fragOffset.y * cameraUpWorld;

    gl_Position = globalUbo.projMatrix * (globalUbo.viewMatrix * vec4(posWorld, 1.0));
}