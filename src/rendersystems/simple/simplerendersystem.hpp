#ifndef SIMPLERENDERSYSTEM_HPP
#define SIMPLERENDERSYSTEM_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <vector>
#include <array>

#include "../../utils/device/device.hpp"
#include "../../utils/pipeline/pipeline.hpp"
#include "../../utils/gameobject/gameobject.hpp"
#include "../../utils/camera/camera.hpp"

namespace Engine {
    class SimpleRenderSystem {
    public:
        SimpleRenderSystem(Device &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects, const Camera &camera);
    private:
        Device &device;
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;

        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);
    };
}

#endif