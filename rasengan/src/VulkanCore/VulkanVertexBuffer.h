#pragma once
#include <vulkan/vulkan.h>

#include <cstdint>
#include "vector"
#include "VulkanVertex.h"

class VulkanVertexBuffer {
public:
    virtual ~VulkanVertexBuffer();

    VulkanVertexBuffer(void* vertices, uint32_t bufferSize);
    VkBuffer  GetVulkanBuffer() const { return vertexBuffer; }
	void UpdateMemory();
private:
	void* vertices;
	uint32_t size ;
    VkBuffer vertexBuffer= VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory= VK_NULL_HANDLE;

};
