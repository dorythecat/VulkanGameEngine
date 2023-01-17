#version 460

const vec2 OFFSETS[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
);

layout (location = 0) out vec2 fragOffset;

layout (set = 0, binding = 0) uniform GlobalUbo {
    mat4 viewMatrix;
    mat4 projMatrix;
    // vec3 lightDir;
    vec4 ambientLightColor;

    vec3 lightPosition;
    vec4 lightColor;
} globalUbo;

const float radius = 0.05;

void main() {
    fragOffset = OFFSETS[gl_VertexIndex];
    vec3 cameraRightWorld = {globalUbo.viewMatrix[0][0], globalUbo.viewMatrix[1][0], globalUbo.viewMatrix[2][0]};
    vec3 cameraUpWorld = {globalUbo.viewMatrix[0][1], globalUbo.viewMatrix[1][1], globalUbo.viewMatrix[2][1]};

    vec3 posWorld = globalUbo.lightPosition.xyz
    + radius * fragOffset.x * cameraRightWorld
    + radius * fragOffset.y * cameraUpWorld;

    gl_Position = globalUbo.projMatrix * (globalUbo.viewMatrix * vec4(posWorld, 1.0));
}