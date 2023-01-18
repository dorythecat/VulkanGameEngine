#version 460

layout (location = 0) in vec2 fragOffset;

layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform GlobalUbo {
    mat4 viewMatrix;
    mat4 projMatrix;
    // vec3 lightDir;
    vec4 ambientLightColor;

    vec3 lightPosition;
    vec4 lightColor;
} globalUbo;

void main() {
    if(sqrt(dot(fragOffset, fragOffset)) + 0.5 >= 1.0) discard;
    outColor = vec4(globalUbo.ambientLightColor.rgb, 1.0);
}