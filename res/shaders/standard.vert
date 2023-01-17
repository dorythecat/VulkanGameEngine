#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec3 fragPos;
layout (location = 2) out vec3 fragNormal;

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
    vec4 worldPos = push.modelMatrix * vec4(position, 1.0);
    gl_Position = globalUbo.viewMatrix * (globalUbo.projMatrix * worldPos);

    fragPos = worldPos.xyz;
    fragNormal = normalize(mat3(push.normalMatrix) * normal);

    fragColor = color;
}