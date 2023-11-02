#version 460

struct PointLight {
    vec4 position;
    vec4 color;
};

layout (constant_id = 0) const uint MAX_POINT_LIGHTS = 8;

layout (set = 0, binding = 0) uniform GlobalUbo {
    mat4 projMatrix;
    mat4 viewMatrix;
    mat4 inverseViewMatrix;

    vec4 ambientLightColor;

    PointLight pointLights[MAX_POINT_LIGHTS];
    uint pointLightCount;

    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;

    bool texturesEnabled;
} globalUbo;

layout (set = 1, binding = 0) uniform sampler2D diffuseMap;

layout (push_constant) uniform PushConstant {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPos;
layout (location = 2) in vec3 fragNormal;
layout (location = 3) in vec2 fragTexCoords;

layout (location = 0) out vec4 outColor;

void main() {
    vec3 diffuse = globalUbo.ambientStrength * globalUbo.ambientLightColor.rgb * globalUbo.ambientLightColor.a;
    vec3 specular = vec3(0.0);
    vec3 surfaceNormal = normalize(fragNormal);

    vec3 cameraPosWorld = globalUbo.inverseViewMatrix[3].xyz;
    vec3 viewDir = normalize(cameraPosWorld - fragPos);

    for(int i = 0; i < globalUbo.pointLightCount; i++) {
        PointLight light = globalUbo.pointLights[i];
        vec3 directionToLight = light.position.xyz - fragPos;

        vec3 intensity = light.color.xyz * light.color.w / dot(directionToLight, directionToLight);

        directionToLight = normalize(directionToLight);
        vec3 halfwayDir = normalize(directionToLight + viewDir);

        diffuse += globalUbo.diffuseStrength * intensity * max(dot(surfaceNormal, directionToLight), 0.0); // lambertian term
        specular += globalUbo.specularStrength * intensity *
                    pow(clamp(dot(surfaceNormal, halfwayDir), 0.0, 1.0), globalUbo.shininess); // blinn-phong term
    }

    if (globalUbo.texturesEnabled) diffuse *= texture(diffuseMap, fragTexCoords).xyz;
    outColor = vec4(diffuse + specular * fragColor, 1.0);
}