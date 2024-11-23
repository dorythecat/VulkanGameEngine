#include "simplerendersystem.hpp"

namespace Engine {
    void SimpleRenderSystem::render(FrameInfo &frameInfo) {
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
            auto &ent = kv.second; // Extract the second element of the pair (the GameObject)
            if (!ent.hasComponent(ComponentType::MODEL) || ent.hasComponent(ComponentType::TEXTURE)) continue;

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