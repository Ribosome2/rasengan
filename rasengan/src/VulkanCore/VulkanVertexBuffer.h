#pragma once
#include <vulkan/vulkan.h>

#include <cstdint>

class VulkanVertexBuffer {
public:
    virtual ~VulkanVertexBuffer();

    VulkanVertexBuffer(void* data, uint32_t size);
    VkBuffer GetVulkanBuffer() const { return vertexBuffer; }
private:
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

};
