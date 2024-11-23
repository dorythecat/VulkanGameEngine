#ifndef TEXTURERENDERSYSTEM_HPP
#define TEXTURERENDERSYSTEM_HPP

#include "../rendersystem.hpp"

namespace Engine {
    class TextureRenderSystem : RenderSystem {
    public:
        TextureRenderSystem(Device &device,
                            VkRenderPass renderPass,
                            VkDescriptorSetLayout globalSetLayout) :
                RenderSystem(device,
                             renderPass,
                             globalSetLayout) { init(); }

        void render(FrameInfo &frameInfo) override;
        using RenderSystem::toggleWireframe;
    private:
        const std::string vertPath() override { return "../res/shaders/compiled/texture.vert.spv"; }
        const std::string fragPath() override { return "../res/shaders/compiled/texture.frag.spv"; }

        std::unique_ptr<DescriptorSetLayout> renderSystemLayout;

        void createPipelineLayout() override;
    };
}

#endif