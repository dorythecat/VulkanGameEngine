#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cassert>
#include <cstring>

#include "../device/device.hpp"

namespace Engine {
    class Buffer {
    public:
        Buffer(Device& device,
               VkDeviceSize instanceSize,
               uint32_t instanceCount,
               VkBufferUsageFlags usageFlags,
               VkMemoryPropertyFlags memoryPropertyFlags,
               VkDeviceSize minOffsetAlignment = 1);
        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void unmap();

        void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void writeToIndex(void* data, uint64_t index);
        VkResult flushIndex(uint64_t index);
        VkDescriptorBufferInfo descriptorInfoForIndex(uint64_t index);
        VkResult invalidateIndex(uint64_t index);

        VkBuffer getBuffer() const { return buffer; }
        void* getMappedMemory() const { return mapped; }
        uint32_t getInstanceCount() const { return instanceCount; }
        VkDeviceSize getInstanceSize() const { return instanceSize; }
        VkDeviceSize getAlignmentSize() const { return alignmentSize; }
        VkBufferUsageFlags getUsageFlags() const { return usageFlags; }
        VkMemoryPropertyFlags getMemoryPropertyFlags() const { return memoryPropertyFlags; }
        VkDeviceSize getBufferSize() const { return bufferSize; }

    private:
        static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);

        Device& device;
        void* mapped = nullptr;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;

        VkDeviceSize bufferSize;
        uint32_t instanceCount;
        VkDeviceSize instanceSize;
        VkDeviceSize alignmentSize;
        VkBufferUsageFlags usageFlags;
        VkMemoryPropertyFlags memoryPropertyFlags;
    };
}

#endif