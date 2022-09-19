#include "VulkanVertexBuffer.h"
#include <vulkan/vulkan.h>
#include "VulkanContext.h"
#include "VulkanBufferHelper.h"
#define  USE_STAGING_BUFFER   //toggle between staging and not staging
VulkanVertexBuffer::VulkanVertexBuffer(void* vertices, uint32_t bufferSize) {
    this->vertices = vertices;
	this->size= bufferSize;
#ifdef USE_STAGING_BUFFER
    auto device = VulkanContext::Get()->VulkanDevice->device;
    VkBuffer stagingBuffer ;
    VkDeviceMemory stagingBufferMemory;
    VulkanBufferHelper::CreateBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer,stagingBufferMemory);

    void* data;
    vkMapMemory(device,stagingBufferMemory,0,bufferSize,0,&data);
    memcpy(data,vertices,(size_t)bufferSize);
    vkUnmapMemory(device,stagingBufferMemory);

    VulkanBufferHelper::CreateBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 vertexBuffer,vertexBufferMemory);

    VulkanBufferHelper::CopyBuffer(stagingBuffer,vertexBuffer,bufferSize);

    //After copying the data from the staging buffer to the device buffer, we should clean it up:
    vkDestroyBuffer(device,stagingBuffer, nullptr);
    vkFreeMemory(device,stagingBufferMemory, nullptr);
#else
    VulkanBufferHelper::CreateBuffer(size,VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                     vertexBuffer,vertexBufferMemory);
	UpdateMemory();
#endif
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
    auto device = VulkanContext::Get()->VulkanDevice->device;

    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);

}

void VulkanVertexBuffer::UpdateMemory() {
#ifndef USE_STAGING_BUFFER  // update memory created with staging buffer will cause Validation Error
	auto  vulkanDevice = VulkanContext::Get()->VulkanDevice;
	auto device = vulkanDevice->device;
	void* data;
	vkMapMemory(device,vertexBufferMemory,0,this->size,0,&data);
	memcpy(data,vertices,(size_t)this->size);
	vkUnmapMemory(device,vertexBufferMemory);
#endif
}

