#ifndef GAME_ENGINE_SWAPCHAIN_HPP
#define GAME_ENGINE_SWAPCHAIN_HPP

#include <vulkan/vulkan.h>

#include <limits>
#include <string>
#include <vector>
#include <array>
#include <memory>

#include "../device/device.hpp"

#include "../image/image.hpp"

namespace Engine {
    class SwapChain {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 3; // Min. 2

        SwapChain(Device &deviceRef, VkExtent2D windowExtent);
        SwapChain(Device &deviceRef, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);
        ~SwapChain() { del(); }

        SwapChain(const SwapChain &) = delete;
        SwapChain& operator=(const SwapChain &) = delete;

        void del();

        [[nodiscard]] VkFramebuffer getFrameBuffer(uint32_t index) const { return swapChainFramebuffers[index]; }
        [[nodiscard]] VkRenderPass getRenderPass() const { return renderPass; }
        [[nodiscard]] VkImageView getImageView(uint32_t index) const { return swapChainImageViews[index]; }
        [[nodiscard]] size_t imageCount() const { return swapChainImages.size(); }
        [[nodiscard]] VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; }
        [[nodiscard]] VkExtent2D getSwapChainExtent() const { return swapChainExtent; }
        [[nodiscard]] uint32_t width() const { return swapChainExtent.width; }
        [[nodiscard]] uint32_t height() const { return swapChainExtent.height; }

        [[nodiscard]] bool compareSwapFormats(const SwapChain &other) const {
            return swapChainImageFormat == other.swapChainImageFormat &&
                   swapChainDepthFormat == other.swapChainDepthFormat;
                   // swapChainExtent.width == other.swapChainExtent.width &&
                   // swapChainExtent.height == other.swapChainExtent.height;
        }

        [[nodiscard]] float extentAspectRatio() const {
            return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
        }
        [[nodiscard]] VkFormat findDepthFormat();

        [[nodiscard]] VkResult acquireNextImage(uint32_t *imageIndex);
        [[nodiscard]] VkResult submitCommandBuffers(const VkCommandBuffer *buffers, const uint32_t *imageIndex);

    private:
        VkFormat swapChainImageFormat;
        VkFormat swapChainDepthFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;

        std::vector<std::unique_ptr<Image>> colorImages;
        std::vector<VkImageView> colorImageViews;
        std::vector<std::unique_ptr<Image>> depthImages;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        Device &device;
        VkExtent2D windowExtent;

        VkSwapchainKHR swapChain;
        std::shared_ptr<SwapChain> oldSwapChain;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;

        void init();
        void createSwapChain();
        void createImageViews();
        void createColorResources();
        void createDepthResources();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        // Helper functions
        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
    };
}

#endif
