#include "renderer.hpp"

// TODO(Dory): Add error codes TO ALL CODE to make debugging easier and faster
// That is gonna take a looong time...
namespace Engine {
    Renderer::Renderer(Window &window, Device &device) : window(window), device(device) {
        recreateSwapChain();
        createCommandBuffers();
    }
    void Renderer::del() {
        freeCommandBuffers();
        swapChain->del();
    }

    VkCommandBuffer Renderer::beginFrame () {
        assert(!isFrameStarted && "Cannot start a frame before ending the previous one!");

        auto result = swapChain->acquireNextImage(&currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            throw std::runtime_error("Failed to acquire the swap chain image!");

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("Failed to begin recording the command buffer!");

        return commandBuffer;
    }
    void Renderer::endFrame () {
        assert(isFrameStarted && "Cannot end a frame before we have started one!");

        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
            throw std::runtime_error("Failed to record the command buffer!");

        auto result = swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()) {
            window.resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) throw std::runtime_error("Failed to present the swap chain image!");

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Cannot begin the render pass outside of a frame!");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot start the render pass on a command buffer from another frame!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(static_cast<uint32_t>(currentImageIndex));

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.01f, 0.01f, 0.01f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }
    void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) const {
        assert(isFrameStarted && "Cannot end the render pass outside of a frame!");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot end the render pass on a command buffer from another frame!");
        vkCmdEndRenderPass(commandBuffer);
    }

    void Renderer::createCommandBuffers() {
            commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = device.getCommandPool();
            allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

            if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
                throw std::runtime_error("Failed to allocate command buffers!");
    }
    void Renderer::recreateSwapChain() {
        auto extent = window.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = window.getExtent();
            glfwWaitEvents();
        } vkDeviceWaitIdle(device.device());

        if (swapChain == nullptr) swapChain = std::make_unique<SwapChain>(device, extent);
        else {
            std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);
            swapChain = std::make_unique<SwapChain>(device, extent, oldSwapChain);
            if(!oldSwapChain->compareSwapFormats(*swapChain))
                throw std::runtime_error("Swap chain image/depth format or presentation mode has changed!");
        }
    }
    void Renderer::freeCommandBuffers() {
            vkFreeCommandBuffers(device.device(),
                                 device.getCommandPool(),
                                 static_cast<uint32_t>(commandBuffers.size()),
                                 commandBuffers.data());
            commandBuffers.clear();
    }
}