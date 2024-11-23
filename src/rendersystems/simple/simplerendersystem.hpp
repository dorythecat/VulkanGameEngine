#ifndef SIMPLERENDERSYSTEM_HPP
#define SIMPLERENDERSYSTEM_HPP

#include "../rendersystem.hpp"

namespace Engine {
    class SimpleRenderSystem : RenderSystem {
    public:
        SimpleRenderSystem(Device &device,
                           VkRenderPass renderPass,
                           VkDescriptorSetLayout globalSetLayout) :
                RenderSystem(device,
                             renderPass,
                             globalSetLayout) { init(); }

        void render(FrameInfo &frameInfo) override;
        using RenderSystem::toggleWireframe;
    private:
        const std::string vertPath() override { return "../res/shaders/compiled/standard.vert.spv"; }
        const std::string fragPath() override { return "../res/shaders/compiled/standard.frag.spv"; }
    };
}

#endif