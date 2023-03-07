#ifndef GAME_ENGINE_DEVICE_HPP
#define GAME_ENGINE_DEVICE_HPP

#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vulkan/vulkan.h>

#include "../window/window.hpp"

namespace Engine {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
        uint32_t graphicsFamily;
        uint32_t presentFamily;

        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
        bool isComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };

    class Device {
    public:
#ifdef DEBUG
        const bool enableValidationLayers = true;
#else
        const bool enableValidationLayers = false;
#endif

        VkPhysicalDeviceProperties properties;

        explicit Device(Window &window);
        ~Device();

        Device(const Device &) = default;
        Device& operator=(const Device &) = delete;
        Device(Device &&) = delete;
        Device& operator=(Device &&) = delete;

        VkCommandPool getCommandPool() const { return commandPool; }
        VkDevice device() { return device_; }
        VkSurfaceKHR surface() const { return surface_; }
        VkQueue graphicsQueue() { return graphicsQueue_; }
        VkQueue presentQueue() { return presentQueue_; }

        VkFormatProperties getFormatProperties(VkFormat format) const {
            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);
            return formatProperties;
        }

        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        // Buffer Helper Functions
        void createBuffer(
                VkDeviceSize size,
                VkBufferUsageFlags usage,
                VkMemoryPropertyFlags properties,
                VkBuffer &buffer,
                VkDeviceMemory &bufferMemory);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

        void createImageWithInfo(const VkImageCreateInfo &imageInfo,
                                 VkMemoryPropertyFlags properties,
                                 VkImage &image,
                                 VkDeviceMemory &imageMemory);

    private:
        const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        Window &window;
        VkCommandPool commandPool;

        VkDevice device_;
        VkSurfaceKHR surface_;
        VkQueue graphicsQueue_;
        VkQueue presentQueue_;

        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();

        // helper functions
        uint32_t rateDeviceSuitability(VkPhysicalDevice device);
        std::vector<const char *> getRequiredExtensions() const;
        bool checkValidationLayerSupport();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void hasGflwRequiredInstanceExtensions();
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    };
} // Engine

#endif
