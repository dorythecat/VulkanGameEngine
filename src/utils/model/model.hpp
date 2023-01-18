#ifndef MODEL_HPP
#define MODEL_HPP

#include "../../../libs/tinyobjloader/tiny_obj_loader.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "../utils.hpp"
#include "../device/device.hpp"
#include "../buffer/buffer.hpp"

// TODO(Dory): Add support for importing .stl and .3mf files
// This are the ones I care about, rest can be added later, or never, I don't care, LULZ

// TODO(Dory): Add support for textures
// TODO(Dory): Add support for normals
// TODO(Dory): Add support for tangents and bitangents
namespace Engine {
    class Model {
    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;
            glm::vec3 normal;
            glm::vec2 texCoord;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &other) const {
                return position == other.position &&
                       color == other.color &&
                       normal == other.normal &&
                       texCoord == other.texCoord;
            }
        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void loadModel(const std::string &path);
        };

        Model(Device &device, const Model::Builder &builder);
        ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        static std::unique_ptr<Model> createModelFromFile(Device &device, const std::string &path);

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer) const;
    private:
        Device device;

        std::unique_ptr<Buffer> vertexBuffer;
        uint32_t vertexCount;

        bool hasIndexBuffer = false;
        std::unique_ptr<Buffer> indexBuffer;
        uint32_t indexCount;

        void createVertexBuffer(const std::vector<Vertex> &vertices);
        void createIndexBuffer(const std::vector<uint32_t> &indices);
    };
}

#endif