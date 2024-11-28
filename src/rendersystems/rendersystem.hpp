#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <vector>
#include <array>
#include <ranges>

#include "../utils/device/device.hpp"
#include "../utils/pipeline/pipeline.hpp"
#include "../utils/entity/entity.hpp"
#include "../utils/camera/camera.hpp"
#include "../utils/frameinfo/frameinfo.hpp"

namespace Engine {
    class RenderSystem {
    public:
        RenderSystem(Device &device,
                     VkRenderPass renderPass,
                     VkDescriptorSetLayout globalSetLayout) :
                     device(device),
                     renderPass(renderPass),
                     globalSetLayout(globalSetLayout) {}
        virtual ~RenderSystem() {
            vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
        }

        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem&) = delete;

        void init() {
            createPipelineLayout();
            createPipeline();
        }

        virtual void render(FrameInfo &frameInfo) = 0;
        void toggleWireframe() {
            wireframe = !wireframe;
            vkDeviceWaitIdle(device.device()); // Wait until the pipeline is no longer on use
            createPipeline();
        }
    protected:
        Device &device;
        VkRenderPass renderPass;
        VkDescriptorSetLayout globalSetLayout;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;

        virtual std::string vertPath() = 0;
        virtual std::string fragPath() = 0;

        bool wireframe = false;
        bool alphaBlending = false;

        virtual void createPipelineLayout() {
            // This is for push constants
            VkPushConstantRange pushConstantRange{};
            pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            pushConstantRange.offset = 0;
            pushConstantRange.size = sizeof(PushConstantData);

            std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

            VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
            pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
            pipelineLayoutInfo.pushConstantRangeCount = 1;
            pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
            if (vkCreatePipelineLayout(device.device(),
                                       &pipelineLayoutInfo,
                                       nullptr,
                                       &pipelineLayout) != VK_SUCCESS)
                throw std::runtime_error("Failed to create the pipeline layout!");
        }
        void createPipeline() {
            assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");
            if (pipeline != nullptr) pipeline = nullptr; // Make sure to destroy any previous pipeline

            PipelineConfigInfo pipelineConfig{};
            Pipeline::defaultPipelineConfigInfo(pipelineConfig);
            if (alphaBlending) Pipeline::enableAlphaBlending(pipelineConfig);
            Pipeline::setSampleCount(pipelineConfig, device.getDesiredSampleCount());
            if (wireframe) Pipeline::enableWireframe(pipelineConfig);
            pipelineConfig.renderPass = renderPass;
            pipelineConfig.pipelineLayout = pipelineLayout;
            pipeline = std::make_unique<Pipeline>(device,
                                                  vertPath(),
                                                  fragPath(),
                                                  pipelineConfig);
        }
    };
}

#endif
