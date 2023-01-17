#include "billboardrendersystem.hpp"

namespace Engine {
    // struct SimplePushConstantData {
    //     glm::mat4 modelMatrix{1.0f};
    //     glm::mat4 normalMatrix{1.0f};
    // };

    BillboardRenderSystem::BillboardRenderSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : device(device) {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }
    BillboardRenderSystem::~BillboardRenderSystem() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void BillboardRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        // This is for push constants
        // VkPushConstantRange pushConstantRange{};
        // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        // pushConstantRange.offset = 0;
        // pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create pipeline layout!");
    }
    void BillboardRenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != NULL && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device,
                                              "../res/shaders/compiled/billboard.vert.spv",
                                              "../res/shaders/compiled/billboard.frag.spv",
                                              pipelineConfig);
    }
    void BillboardRenderSystem::render(FrameInfo &frameInfo) {
        pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(
                frameInfo.commandBuffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipelineLayout,
                0,
                1,
                &frameInfo.globalDescriptorSet,
                0,
                nullptr);

        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
}