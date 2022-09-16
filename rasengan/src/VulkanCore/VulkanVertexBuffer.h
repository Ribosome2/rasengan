#pragma once
#include <vulkan/vulkan.h>

#include <cstdint>
#include "vector"
#include "VulkanVertex.h"

class VulkanVertexBuffer {
public:
    virtual ~VulkanVertexBuffer();

    VulkanVertexBuffer(std::vector<VulkanVertex> & vertices, uint32_t size);
    VkBuffer  GetVulkanBuffer() const { return vertexBuffer; }
private:
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

};
