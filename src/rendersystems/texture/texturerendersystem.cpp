#include "texturerendersystem.hpp"
#include "../../utils/entity/components/model.hpp"

namespace Engine {
    TextureRenderSystem::TextureRenderSystem(Device &device,
                                                VkRenderPass renderPass,
                                                VkDescriptorSetLayout globalSetLayout)
                                                : device {device} {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }
    TextureRenderSystem::~TextureRenderSystem() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void TextureRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        // This is for push constants
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        renderSystemLayout = DescriptorSetLayout::Builder(device)
                .addBinding(0,
                            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                            VK_SHADER_STAGE_FRAGMENT_BIT).build();

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{
            globalSetLayout,
            renderSystemLayout->getDescriptorSetLayout()
        };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create the pipeline layout!");
    }

    void TextureRenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        Pipeline::setSampleCount(pipelineConfig, device.getDesiredSampleCount());
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(device,
                                              "../res/shaders/compiled/texture.vert.spv",
                                              "../res/shaders/compiled/texture.frag.spv",
                                              pipelineConfig);
    }

    void TextureRenderSystem::render(FrameInfo &frameInfo) {
        pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout,
                                0,
                                1,
                                &frameInfo.globalDescriptorSet,
                                0,
                                nullptr);

        for (auto &kv : frameInfo.entities) {
            auto &ent = kv.second;
            if (!ent.hasComponent(ComponentType::MODEL) || !ent.hasComponent(ComponentType::TEXTURE)) continue;

            VkDescriptorSet descriptorSet;
            auto imageInfo = ent.getTextureComponent()->diffuseMap->getDescriptorImageInfo();
            DescriptorWriter(*renderSystemLayout, frameInfo.frameDescriptorPool)
                    .writeImage(0, &imageInfo)
                    .build(descriptorSet);

            vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    pipelineLayout,
                                    1,
                                    1,
                                    &descriptorSet,
                                    0,
                                    nullptr);

            PushConstantData push{};
            push.modelMatrix = ent.getTransformComponent()->mat4();
            push.normalMatrix = ent.getTransformComponent()->normal();

            vkCmdPushConstants(frameInfo.commandBuffer,
                               pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0,
                               sizeof(PushConstantData),
                               &push);

            ent.getModelComponent()->model->bind(frameInfo.commandBuffer);
            ent.getModelComponent()->model->draw(frameInfo.commandBuffer);
        }
    }
}