#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texCoord;

layout (location = 0) out vec3 fragColor;

layout (push_constant) uniform PushConstant {
    mat4 transform; // projection * view * model
    mat4 normal;
} push;

const vec3 lightdir = normalize(vec3(1.0, -3.0, -1.0));
const float ambient = 0.05;

void main() {
    gl_Position = push.transform * vec4(position, 1.0);

    vec3 normalWorldSpace = normalize(mat3(push.normal) * normal);

    float lightIntensity = ambient + max(dot(normalWorldSpace, lightdir), 0.0);

    fragColor = lightIntensity * color;
}