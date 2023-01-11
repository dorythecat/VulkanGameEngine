#ifndef MODEL_HPP
#define MODEL_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "../device/device.hpp"

// TODO: Add support for importing .obj, .stl and .3mf files
// This are the ones I care about, rest can be added later, or never, I don't care, LULZ

// TODO: Add support for textures
// TODO: Add support for normals
// TODO: Add support for tangents and bitangents
namespace Engine {
    class Model {
    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};
        };

        Model(Device &device, const Model::Builder &builder);
        ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
    private:
        Device device;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;

        bool hasIndexBuffer = false;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        uint32_t indexCount;

        void createVertexBuffer(const std::vector<Vertex> &vertices);
        void createIndexBuffer(const std::vector<uint32_t> &indices);
    };
}

#endif