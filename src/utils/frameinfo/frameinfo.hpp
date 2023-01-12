#ifndef FRAMEINFO_HPP
#define FRAMEINFO_HPP

#include <vulkan/vulkan.h>

#include "../camera/camera.hpp"

namespace Engine {
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera camera;
        VkDescriptorSet globalDescriptorSet;
    };
}

#endif
