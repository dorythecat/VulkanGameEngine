#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stb_image.h>

#include <stdexcept>
#include <cstdint>

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

        void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkBuffer buffer);
        VkImageView createImageView();

    private:
        Device &device;

        VkImage image;
        VkDeviceMemory imageMemory;

        VkFormat format;

        uint32_t width;
        uint32_t height;
    };
}

#endif