#pragma once


#include <cstdint>
#include <vulkan/vulkan_core.h>

class VulkanIndexBuffer {
public:
	VulkanIndexBuffer(void* indices,size_t indexCount,size_t bufferSize);
	VkBuffer & GetIndexBuffer(){ return indexBuffer;}
	uint32_t GetCount() const  { return indicesCount; }

	virtual ~VulkanIndexBuffer();

private:
	void* indices;
	uint32_t bufferSize=0 ;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	uint32_t indicesCount=0;
};
