#pragma once
#include <vulkan/vulkan.h>

#include <cstdint>
#include "vector"
#include "VulkanVertex.h"

class VulkanVertexBuffer {
public:
    virtual ~VulkanVertexBuffer();

    VulkanVertexBuffer(void* vertices, uint32_t size);
    VkBuffer  GetVulkanBuffer() const { return vertexBuffer; }
	void UpdateMemory();
private:
	void* vertices;
	uint32_t size ;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

};
