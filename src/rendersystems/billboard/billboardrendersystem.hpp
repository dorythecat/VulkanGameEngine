#ifndef BILLBOARDRENDERSYSTEM_HPP
#define BILLBOARDRENDERSYSTEM_HPP

#include "../rendersystem.hpp"

namespace Engine {
    class BillboardRenderSystem : RenderSystem {
    public:
        BillboardRenderSystem(Device &device,
                              VkRenderPass renderPass,
                              VkDescriptorSetLayout globalSetLayout) :
                RenderSystem(device,
                             renderPass,
                             globalSetLayout) {
            alphaBlending = true;
            init();
        }

        static void update(const FrameInfo &frameInfo, GlobalUbo &ubo);
        void render(FrameInfo &frameInfo) override;
        // We don't include the wireframe function here because that wouldn't really be useful anyways
    private:
        const std::string vertPath() override { return "../res/shaders/compiled/billboard.vert.spv"; }
        const std::string fragPath() override { return "../res/shaders/compiled/billboard.frag.spv"; }
    };
}

#endif