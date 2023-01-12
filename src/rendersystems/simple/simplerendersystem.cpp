#include "simplerendersystem.hpp"

namespace Engine {
    struct SimplePushConstantData {
        glm::mat4 transform{1.0f};
        glm::mat4 normal{1.0f};
    };

    SimpleRenderSystem::SimpleRenderSystem(Device &device, VkRenderPass renderPass) : device(device) {
        createPipelineLayout();
        createPipeline(renderPass);
    }
    SimpleRenderSystem::~SimpleRenderSystem() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void SimpleRenderSystem::createPipelineLayout() {
        // This is for push constants
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create pipeline layout!");
    }
    void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != NULL && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device,
                                              "../res/shaders/compiled/vert.spv",
                                              "../res/shaders/compiled/frag.spv",
                                              pipelineConfig);
    }
    void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer,
                                               std::vector<GameObject> &gameObjects,
                                               const Camera &camera) {
        pipeline->bind(commandBuffer);

        auto projection = camera.getProjectionMatrix() * camera.getViewMatrix();
        for (auto &obj : gameObjects) {
            SimplePushConstantData push{};
            push.transform = projection * obj.transform.mat4();
            push.normal = obj.transform.normal();

            vkCmdPushConstants(commandBuffer,
                               pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0,
                               sizeof(SimplePushConstantData),
                               &push);

            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
}