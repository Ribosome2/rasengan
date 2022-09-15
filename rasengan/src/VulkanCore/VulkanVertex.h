#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include "array"
class VulkanVertex {
public:
    glm::vec2 pos;
    glm::vec3 color;
public:
    static VkVertexInputBindingDescription GetBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

};
