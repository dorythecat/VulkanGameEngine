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
              VkFormat format,
              VkImageTiling tiling,
              VkImageUsageFlags usage,
              VkMemoryPropertyFlags properties);
        ~Image();

        Image(const Image &) = delete;
        Image operator=(const Image &) = delete;
        Image(Image &&) = delete;
        Image& operator=(Image &&) = delete;

        uint32_t getMipLevels() const { return mipLevels; }

        void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer);
        void generateMipmaps();
        VkImageView createImageView();
    private:
        Device &device;

        VkImage image;
        VkDeviceMemory imageMemory;

        uint32_t width;
        uint32_t height;

        VkFormat format;
        VkImageTiling tiling;
        VkImageUsageFlags usage;
        VkMemoryPropertyFlags properties;

        uint32_t mipLevels;

        void createImage();
    };
}

#endif