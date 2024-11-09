#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stb_image.h>

#include <stdexcept>
#include <cstdint>
#include <memory>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "../device/device.hpp"
#include "../buffer/buffer.hpp"

namespace Engine {
    class Image {
    public:
        Image(Device &device,
              uint32_t width,
              uint32_t height,
              VkSampleCountFlagBits numSamples,
              VkFormat format,
              VkImageTiling tiling,
              VkImageUsageFlags usage,
              VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        ~Image() { del(); }

        Image(const Image &) = delete;
        Image operator=(const Image &) = delete;
        Image(Image &&) = delete;
        Image& operator=(Image &&) = delete;

        uint32_t getMipLevels() const { return mipLevels; }

        void del();

        void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer);
        void generateMipmaps();
        VkImageView createImageView(VkImageAspectFlags aspectFlags);
    private:
        Device &device;

        VkImage image;
        VkImageView imageView = nullptr;
        VkDeviceMemory imageMemory;

        uint32_t width;
        uint32_t height;
        VkSampleCountFlagBits numSamples;

        VkFormat format;
        VkImageTiling tiling;
        VkImageUsageFlags usage;
        VkMemoryPropertyFlags properties;

        uint32_t mipLevels;

        void createImage();
    };
}

#endif