#ifndef GAME_ENGINE_DEVICE_HPP
#define GAME_ENGINE_DEVICE_HPP

#define DESIRED_SAMPLE_COUNT VK_SAMPLE_COUNT_8_BIT

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
        constexpr static bool enableValidationLayers = true;
#else
        constexpr static bool enableValidationLayers = false;
#endif

        VkPhysicalDeviceProperties properties;

        explicit Device(Window &window);
        ~Device();

        Device(const Device &) = default;
        Device& operator=(const Device &) = delete;
        Device(Device &&) = delete;
        Device& operator=(Device &&) = delete;

        VkCommandPool getCommandPool() const { return _commandPool; }
        VkInstance instance() { return _instance; }
        VkDevice device() { return _device; }
        VkPhysicalDevice physicalDevice() { return _physicalDevice; }
        VkSurfaceKHR surface() const { return _surface; }
        VkQueue graphicsQueue() { return _graphicsQueue; }
        VkQueue presentQueue() { return _presentQueue; }

        VkFormatProperties getFormatProperties(VkFormat format) const {
            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &formatProperties);
            return formatProperties;
        }

        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(_physicalDevice); }
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(_physicalDevice); }
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        VkSampleCountFlagBits getMaxUsableSampleCount();
        VkSampleCountFlagBits getDesiredSampleCount() { return std::min(DESIRED_SAMPLE_COUNT, getMaxUsableSampleCount()); }

        // Buffer Helper Functions
        void createBuffer(VkDeviceSize size,
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

        void transitionImageLayout(VkImage image,
                                   VkFormat format,
                                   VkImageLayout oldLayout,
                                   VkImageLayout newLayout,
                                   uint32_t mipLevels = 1,
                                   uint32_t layerCount = 1);
    private:
        const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        VkDebugUtilsMessengerEXT debugMessenger;
        Window &window;

        VkCommandPool _commandPool;
        VkInstance _instance;
        VkDevice _device;
        VkPhysicalDevice _physicalDevice = nullptr;
        VkSurfaceKHR _surface;
        VkQueue _graphicsQueue;
        VkQueue _presentQueue;

        void createInstance();
        void setupDebugMessenger();
        void createSurface() { window.createWindowSurface(_instance, &_surface); }
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createCommandPool();

        // helper functions
        uint32_t rateDeviceSuitability(VkPhysicalDevice device);
        static std::vector<const char*> getRequiredExtensions() ;
        bool checkValidationLayerSupport();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
        void hasGflwRequiredInstanceExtensions();
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    };
}

#endif