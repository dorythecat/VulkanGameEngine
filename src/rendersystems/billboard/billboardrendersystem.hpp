#ifndef BILLBOARDRENDERSYSTEM_HPP
#define BILLBOARDRENDERSYSTEM_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <vector>
#include <array>

#include "../../utils/device/device.hpp"
#include "../../utils/pipeline/pipeline.hpp"
#include "../../utils/entity/entity.hpp"
#include "../../utils/camera/camera.hpp"
#include "../../utils/frameinfo/frameinfo.hpp"

namespace Engine {
    class BillboardRenderSystem {
    public:
        BillboardRenderSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~BillboardRenderSystem();

        BillboardRenderSystem(const BillboardRenderSystem&) = delete;
        BillboardRenderSystem& operator=(const BillboardRenderSystem&) = delete;

        void update(FrameInfo &frameInfo, GlobalUbo &ubo);
        void render(FrameInfo &frameInfo);
    private:
        Device &device;
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);
    };
}

#endif