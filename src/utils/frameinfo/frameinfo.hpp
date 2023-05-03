#ifndef FRAMEINFO_HPP
#define FRAMEINFO_HPP

// Constants
#define MAX_POINT_LIGHTS 8

#include <vulkan/vulkan.h>

#include "../camera/camera.hpp"
#include "../descriptors/descriptors.hpp"
#include "../entity/entity.hpp"

// Alignment requirements need to be met correctly in all buffers, else, weird, un-debuggable errors will occur almost surely
// (See https://registry.khronos.org/vulkan/specs/1.3-extensions/html/chap15.html#interfaces-resources-layout)
namespace Engine {
    struct PointLight {
        glm::vec4 position{}; // ignore w // 16 bytes
        glm::vec4 color{}; // w = intensity // 16 bytes
    };

    struct GlobalUbo {
        glm::mat4 projectionMatrix{1.0f}; // 64 bytes
        glm::mat4 viewMatrix{1.0f}; // 64 bytes
        glm::mat4 inverseViewMatrix{1.0f}; // 64 bytes

        glm::vec4 ambientLightColor{1.0f, 1.0f, 1.0f, 0.05f}; // 16 bytes

        // Lightning
        PointLight pointLights[MAX_POINT_LIGHTS]; // 128 bytes
        uint8_t pointLightCount = 0; // 4 bytes
    };

    struct PushConstantData {
        glm::mat4 modelMatrix{1.0f}; // 64 bytes
        glm::mat4 normalMatrix{1.0f}; // 64 bytes
    };

    struct FrameInfo {
        static constexpr float MAX_DELTA_TIME = 0.0333f; // 30 FPS

        uint32_t frameIndex = 0;
        float frameTime = 0.0f;
        VkCommandBuffer commandBuffer{};
        Camera &camera;
        VkDescriptorSet globalDescriptorSet{};
        DescriptorPool &frameDescriptorPool;  // Descriptor pool, cleared each frame
        Entity::Map &entities;
    };
}

#endif
