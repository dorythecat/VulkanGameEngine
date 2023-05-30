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

layout (push_constant) uniform PushConstant {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec3 fragPos;
layout (location = 2) out vec3 fragNormal;
layout (location = 3) out vec2 fragTexCoord;

void main() {
    vec4 worldPos = push.modelMatrix * vec4(position, 1.0);
    gl_Position = globalUbo.projMatrix * (globalUbo.viewMatrix * worldPos);

    fragPos = worldPos.xyz;
    fragNormal = normalize(mat3(push.normalMatrix) * normal);

    fragColor = color;

    fragTexCoord = vec2(texCoord.x, -texCoord.y);
}