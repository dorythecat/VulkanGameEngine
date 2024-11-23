#include "billboardrendersystem.hpp"

namespace Engine {
    struct PointLightPushConstant {
        glm::vec4 position{};
        glm::vec4 color{};
        float radius = 0.0f;
    };
    void BillboardRenderSystem::update(FrameInfo &frameInfo, GlobalUbo &ubo) {
        Entity::id_t index = 0;
        for (auto &kv : frameInfo.entities) {
            auto &ent = kv.second;
            if(!ent.hasComponent(ComponentType::POINT_LIGHT)) continue;

            ubo.pointLights[index].position = glm::vec4(ent.getTransformComponent()->position, 1.0f);
            ubo.pointLights[index].color = glm::vec4(ent.getPointLightComponent()->color,
                                                     ent.getPointLightComponent()->intensity);
            index++;
        } ubo.pointLightCount = index;
    }
    void BillboardRenderSystem::render(FrameInfo &frameInfo) {
        // Sort the objects from back to front, for alpha blending to work correctly
        // TODO(Dory): Implement Order-Independent rendering so that this isn't necessary
        std::map<float, Entity::id_t> sorted;
        for (auto &kv : frameInfo.entities) {
            auto &ent = kv.second;
            if(!ent.hasComponent(ComponentType::POINT_LIGHT)) continue;

            // We really don't care if the distance is squared, we just care about the order so we can save a sqrt operation
            glm::vec3 offset = frameInfo.camera.getPosition() - ent.getTransformComponent()->position;
            float distance = glm::dot(offset, offset);
            sorted[distance] = ent.getId();
        }

        pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout,
                                0,
                                1,
                                &frameInfo.globalDescriptorSet,
                                0,
                                nullptr);
        for (auto & kv : std::ranges::reverse_view(sorted)) {
            auto &ent = frameInfo.entities.at(kv.second);

            PointLightPushConstant push{};
            push.position = glm::vec4(ent.getTransformComponent()->position, 1.0f);
            push.color = glm::vec4(ent.getPointLightComponent()->color,
                                   ent.getPointLightComponent()->intensity);
            push.radius = ent.getTransformComponent()->scale.x;

            vkCmdPushConstants(frameInfo.commandBuffer,
                               pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                               0,
                               sizeof(PointLightPushConstant),
                               &push);
            vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
        }
    }
}