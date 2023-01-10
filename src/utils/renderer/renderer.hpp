#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <vector>
#include <array>
#include <cassert>

#include "../window/window.hpp"
#include "../device/device.hpp"
#include "../swapchain/swapchain.hpp"

namespace Engine {
    class Renderer {
    public:
        Renderer(Window &window, Device &device);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer outside of a render pass!");
            return commandBuffers[currentFrameIndex];
        }

        int getCurrentFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index outside of a render pass!");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);

        float getAspectRatio() const { return swapChain->extentAspectRatio(); }

        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
    private:
        Window &window;
        Device &device;
        std::unique_ptr<SwapChain> swapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex = 0;
        int currentFrameIndex = 0;
        bool isFrameStarted = false;

        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();
    };
}

#endif