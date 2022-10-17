#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include "array"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

class VulkanVertex {
public:
    bool operator==(const VulkanVertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
public:
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
public:
    static VkVertexInputBindingDescription GetBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();

};

namespace std {
    template<> struct hash<VulkanVertex> {
        size_t operator()(VulkanVertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                     (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}