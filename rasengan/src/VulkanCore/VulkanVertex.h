#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include "array"
class VulkanVertex {
public:
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
public:
    static VkVertexInputBindingDescription GetBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions();

};
