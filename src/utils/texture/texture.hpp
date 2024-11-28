#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <stb_image.h>

#include <stdexcept>
#include <memory>

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>

#include "../device/device.hpp"
#include "../buffer/buffer.hpp"
#include "../image/image.hpp"

namespace Engine {
    class Texture {
    public:
        Texture(Device &device, const char* texturePath);
        ~Texture();

        Texture(const Texture &) = delete;
        Texture& operator=(const Texture &) = delete;
        Texture(Texture &&) = delete;
        Texture& operator=(Texture &&) = delete;

        [[nodiscard]] VkDescriptorImageInfo getDescriptorImageInfo() const;
    private:
        Device &device;
        std::unique_ptr<Image> textureImage;
        VkImageView textureImageView;
        VkSampler textureSampler;

        const char* texturePath;

        void createTextureImage();
        void createTextureSampler();
    };
}

#endif