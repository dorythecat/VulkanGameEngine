#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

layout (location = 0) out vec3 fragColor;

layout (set = 0, binding = 0) uniform GlobalUbo {
    mat4 viewMatrix;
    mat4 projMatrix;
    vec3 lightDir;
} globalUbo;

layout (push_constant) uniform PushConstant {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

const float ambient = 0.05;

void main() {
    gl_Position = globalUbo.viewMatrix * globalUbo.projMatrix * push.modelMatrix * vec4(position, 1.0);

    vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

    float lightIntensity = ambient + max(dot(normalWorldSpace, globalUbo.lightDir), 0.0);

    fragColor = lightIntensity * color;
}