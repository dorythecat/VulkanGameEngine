#include "texture.hpp"

namespace Engine {
    Texture::Texture(Device &device, const char *texturePath) : device(device), texturePath(texturePath) {
        createTextureImage();
        textureImageView = textureImage->createImageView();
        createTextureSampler();
    }

    Texture::~Texture() {
        vkDestroySampler(device.device(), textureSampler, nullptr);
        vkDestroyImageView(device.device(), textureImageView, nullptr);
    };

    void Texture::createTextureImage () {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(texturePath,
                                    &texWidth,
                                    &texHeight,
                                    &texChannels,
                                    STBI_rgb_alpha);
        VkDeviceSize imageSize = 4 * static_cast<VkDeviceSize>(texWidth) * static_cast<VkDeviceSize>(texHeight);

        if (!pixels) throw std::runtime_error("Failed to load the texture image!");

        Buffer stagingBuffer{
                device,
                imageSize,
                1,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer(pixels, imageSize);

        stbi_image_free(pixels);

        textureImage = std::make_unique<Image>(
                device,
                static_cast<uint32_t>(texWidth),
                static_cast<uint32_t>(texHeight),
                VK_FORMAT_R8G8B8A8_SRGB,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        textureImage->transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        textureImage->copyBufferToImage(stagingBuffer.getBuffer());
        textureImage->transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        textureImage->copyBufferToImage(stagingBuffer.getBuffer());
        textureImage->generateMipmaps();
        stagingBuffer.unmap();
    }

    void Texture::createTextureSampler() {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = device.properties.limits.maxSamplerAnisotropy; // Why not?
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f; // Optional
        samplerInfo.maxLod = static_cast<float>(textureImage->getMipLevels());
        samplerInfo.mipLodBias = 0.0f; // Optional

        if (vkCreateSampler(device.device(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
            throw std::runtime_error("Failed to create the texture sampler!");
    }

    VkDescriptorImageInfo Texture::getDescriptorImageInfo () const {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = textureImageView;
        imageInfo.sampler = textureSampler;

        return imageInfo;
    }
}