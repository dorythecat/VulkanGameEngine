#ifndef TEXTURERENDERSYSTEM_HPP
#define TEXTURERENDERSYSTEM_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>
#include "../../utils/frameinfo/frameinfo.hpp"
#include "../../utils/pipeline/pipeline.hpp"

namespace Engine {
    class TextureRenderSystem {
    public:
        TextureRenderSystem(Device &device,
                            VkRenderPass renderPass,
                            VkDescriptorSetLayout globalSetLayout);
        ~TextureRenderSystem();

        TextureRenderSystem(const TextureRenderSystem&) = delete;
        TextureRenderSystem& operator=(const TextureRenderSystem&) = delete;

        void render(FrameInfo &frameInfo);
    private:
        Device &device;
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;

        std::unique_ptr<DescriptorSetLayout> renderSystemLayout;

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);
    };
}

#endif